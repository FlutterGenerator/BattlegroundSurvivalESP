#pragma once

#include <vector>
#include <mutex>
#include <chrono>

#include "Unity/Vector3.h" // Если Vector3.h отсутствует, подключи файл, где объявлен Vector3.

// Функция определена в hooks.h или другом исходном файле.
bool IsPlayerDead(void *player);

extern void *myPlayer;

struct enemy_t {
    void *object;
    Vector3 location;
    int health;
    long long lastSeen; // ms, updated every time the enemy Update hook runs
};

inline long long espNowMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();
}

class ESPManager {
public:
    std::vector<enemy_t *> *enemies;
    std::mutex mtx;

    ESPManager() : enemies(new std::vector<enemy_t *>()) {}

    ~ESPManager() {
        for (enemy_t *enemy : *enemies) {
            delete enemy;
        }
        delete enemies;
    }

    bool isEnemyPresentUnlocked(void *enemyObject) {
        for (enemy_t *enemy : *enemies) {
            if (enemy->object == enemyObject) {
                return true;
            }
        }
        return false;
    }

    bool isEnemyPresent(void *enemyObject) {
        std::lock_guard<std::mutex> lock(mtx);
        return isEnemyPresentUnlocked(enemyObject);
    }

    // copy of object pointers, safe to iterate from the draw thread
    std::vector<void *> snapshot() {
        std::lock_guard<std::mutex> lock(mtx);
        std::vector<void *> out;
        long long now = espNowMs();
        // drop enemies whose Update stopped running (match ended / scene unloaded / destroyed)
        for (auto it = enemies->begin(); it != enemies->end();) {
            if (!*it || now - (*it)->lastSeen > 250) {
                delete *it;
                it = enemies->erase(it);
            } else {
                out.push_back((*it)->object);
                ++it;
            }
        }
        return out;
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mtx);
        for (enemy_t *e : *enemies) delete e;
        enemies->clear();
    }

    void tryAddEnemy(void *enemyObject) {
        std::lock_guard<std::mutex> lock(mtx);
        if (!enemyObject) return;
        long long now = espNowMs();
        for (enemy_t *e : *enemies) {
            if (e->object == enemyObject) {
                e->lastSeen = now;
                return;
            }
        }
        if (IsPlayerDead(enemyObject)) return;

        enemy_t *enemy = new enemy_t{};
        enemy->object = enemyObject;
        enemy->lastSeen = now;
        enemies->push_back(enemy);
    }

    void updateEnemies(void *enemyObject) {
        std::lock_guard<std::mutex> lock(mtx);
        (void)enemyObject;

        for (auto it = enemies->begin(); it != enemies->end();) {
            enemy_t *enemy = *it;

            if (!enemy || IsPlayerDead(enemy->object)) {
                delete enemy;
                it = enemies->erase(it);
            } else {
                ++it;
            }
        }
    }

    void removeEnemy(enemy_t *enemy) {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto it = enemies->begin(); it != enemies->end(); ++it) {
            if (*it == enemy) {
                delete *it;
                enemies->erase(it);
                return;
            }
        }
    }

    void removeEnemyGivenObject(void *enemyObject) {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto it = enemies->begin(); it != enemies->end(); ++it) {
            if ((*it)->object == enemyObject) {
                delete *it;
                enemies->erase(it);
                return;
            }
        }
    }
};