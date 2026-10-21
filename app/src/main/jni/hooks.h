#pragma once

#include <cstdint>
#include <string>
#include "Unity/Call_Unity.h"
#include "NepAU/AutoUpdate.cpp"

using namespace NepSymbol;

lib l2cpp;
Address transformOff,
        getpos, 
        wtsp, 
        getCam, 
        getHp, 
        getMaxHp,
        UpDate,
        Destroyz,
        scrW,
        scrH,
        getGC;
void getAddress() {
    l2cpp.name = "libil2cpp.so";
    l2cpp.open();
    
    Uclass Component = l2cpp.getClass("UnityEngine", "Component");
    transformOff = Component.dump("get_transform");
    Uclass Tranf = l2cpp.getClass("UnityEngine", "Transform");
    getpos = Tranf.dump("get_position_Injected", 1);
    Uclass Cam = l2cpp.getClass("UnityEngine", "Camera");
    wtsp = Cam.dump("WorldToScreenPoint_Injected", 3);
    getCam = Cam.dump("get_main");
    
    Uclass Scr = l2cpp.getClass("UnityEngine", "Screen");
    scrW = Scr.dump("get_width");
    scrH = Scr.dump("get_height");

    Uclass GC = l2cpp.getClass("", "GameController");
    getGC = GC.dump("get_gameController");

    // Enemy class found by name at runtime (works on any game version)
    Uclass AI = l2cpp.getClass("", "AIController");
    UpDate = AI.dump("Update");
    Destroyz = AI.dump("die");
    
}

void *getTransform(void *player) {
    if (!player) return NULL;
    static const auto get_transform_injected = reinterpret_cast<uint64_t( *)(void *)>(transformOff.getAddr());
    return (void *) get_transform_injected(player);
}

Vector3 get_position(void *transform) {
    if (!transform)return Vector3();
    Vector3 position;
    static const auto get_position_injected = reinterpret_cast<uint64_t( *)(void *,Vector3 &)>(getpos.getAddr());
    get_position_injected(transform, position);
    return position;
}
// GameController.Alive (int @0x164) = enemies left in the match; -1 if unavailable
#if defined(__aarch64__)
    constexpr uintptr_t ALIVE_OFFSET = 0x164; // ARM64
#else
    constexpr uintptr_t ALIVE_OFFSET = 0xB8; // ARM32: нужен правильный offset
#endif

int getEnemiesLeft() {
    uintptr_t a = getGC.getAddr();
    if (!a) return -1;

    auto fn = reinterpret_cast<void* (*)()>(a);
    void* gc = fn();
    if (!gc) return -1;

    int v = *reinterpret_cast<int*>(
        reinterpret_cast<uintptr_t>(gc) + ALIVE_OFFSET
    );

    return (v >= 0 && v < 1000) ? v : -1;
}

// game screen -> overlay scale (overlay size / Unity Screen size), set in DrawESP
float gScaleX = 1.0f, gScaleY = 1.0f;
int get_screen_width() {
    uintptr_t a = scrW.getAddr();
    if (!a) return 0;
    return reinterpret_cast<int ( *)()>(a)();
}
int get_screen_height() {
    uintptr_t a = scrH.getAddr();
    if (!a) return 0;
    return reinterpret_cast<int ( *)()>(a)();
}

Vector3 WorldToScreenPoint(void *transform, Vector3 test) {
    if (!transform) return Vector3();
    Vector3 position;
    // Camera.WorldToScreenPoint_Injected(ref Vector3 position, MonoOrStereoscopicEye eye, out Vector3 ret)
    // ref/out = pointers, so pass addresses (not Vector3 by value)
    static const auto WorldToScreenPoint_Injected = reinterpret_cast<void( *)(void *, Vector3 *, int, Vector3 *)>(wtsp.getAddr());
    WorldToScreenPoint_Injected(transform, &test, 4, &position);
    position.x *= gScaleX;
    position.y *= gScaleY;
    return position;
}
void *get_camera() {
    static const auto get_camera_injected = reinterpret_cast<uint64_t( *)()>(getCam.getAddr());
    return (void *) get_camera_injected();
}
Vector3 GetPlayerLocation(void *player) {
    return get_position(getTransform(player));
}

// AIController fields: HEALTH float @0x84, isDead bool @0x1F8
#if defined(__aarch64__)
    constexpr uintptr_t HEALTH_OFFSET = 0x84; // ARM64
#else
    constexpr uintptr_t HEALTH_OFFSET = 0x48; // ARM32: правильный offset
#endif

int GetPlayerHealth(void *e) {
    if (!e) return 0;

    float hp = *reinterpret_cast<float*>(
        reinterpret_cast<uintptr_t>(e) + HEALTH_OFFSET
    );

    return static_cast<int>(hp);
}

int GetMaxHealth(void *e) {
    static int maxHp = 100;

    int cur = GetPlayerHealth(e);
    if (cur > maxHp) maxHp = cur;

    return maxHp;
}

#if defined(__aarch64__)
    constexpr uintptr_t DEAD_OFFSET = 0x1F8; // ARM64
#else
    constexpr uintptr_t DEAD_OFFSET = 0x11C; // ARM32: правильный offset
#endif

bool PlayerAlive(void *e) {
    if (!e) return false;

    bool dead = *reinterpret_cast<bool *>(
        reinterpret_cast<uintptr_t>(e) + DEAD_OFFSET
    );

    return !dead && GetPlayerHealth(e) > 0;
}

bool IsPlayerDead(void *e) {
    return !PlayerAlive(e);
}


int string_to_int(string str) 
{ 
    int value;
    value = std::stoi(str);
    return value;
}
string float_to_string (int value) 
{
    string str; 
    str = std::to_string(value);
    str = "[" + str + "]";
    return str;
}
