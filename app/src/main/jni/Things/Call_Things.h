#pragma once

#include "menu.h"
#include "NepDraw.h"
#include "NepAU/AutoUpdate.cpp"
#include "hooks.h"
#include "ESPManager.h"
#include "dobby/dobby.h"
#include <Substrate/SubstrateHook.h>
#include <Substrate/CydiaSubstrate.h>

ESPManager *espManager = nullptr;
NepEsp es;

bool ESP, ESPLine, ESPBox, ESPBox3d, ESPHealth, ESPDistance, EspObject;
bool setPos = false;
bool PerShot = false;
bool EzBuy = false;
bool EzDrop = false;
bool Ammo = false;
bool GunWalkSpeed = false;
bool xSpam = false;
bool ChangeName = false;
bool HerName = false;
bool Hehe = false;

float SetX, SetY, SetZ = 0;
bool xPos = false;
bool isTarg = false;




void *playerlate = NULL;
void *get_Player(void *player) {
    playerlate = player;
    return playerlate;
}

Vector3 add(Vector3 a, Vector3 b) {
    Vector3 c;
    c.x= a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
    return c;
}

void Draw3dBox(NepEsp esp,Color espColor, Vector3 Transform,void * camera,int glHeight,int glWidth)
{   
    Vector3 position2 = add(Transform, Vector3(0.6, 1.6, 0.6));
    Vector3 position3 = add(Transform, Vector3(0.6, 0, 0.6));
    Vector3 position4 = add(Transform, Vector3(-0.5, 0, 0.6)); 
    Vector3 position5 = add(Transform, Vector3(-0.5, 1.6, 0.6));
    Vector3 position6 = add(add(Transform, Vector3(0.6, 1.6, 0)), Vector3(0, 0, -0.6));
    Vector3 position7 = add(add(Transform, Vector3(0.6, 0, 0)), Vector3(0, 0, -0.6));
    Vector3 position8 = add(add(Transform, Vector3(-0.5, 0, 0)), Vector3(0, 0, -0.6)); 
    Vector3 position9 = add(add(Transform, Vector3(-0.5, 1.6, 0)), Vector3(0, 0, -0.6));


    Vector3 vector = WorldToScreenPoint(camera, position2);
    Vector3 vector2 = WorldToScreenPoint(camera, position3);
    Vector3 vector3 = WorldToScreenPoint(camera, position4);
    Vector3 vector4 = WorldToScreenPoint(camera, position5);
    Vector3 vector5 = WorldToScreenPoint(camera, position6);
    Vector3 vector6 = WorldToScreenPoint(camera, position7);
    Vector3 vector7 = WorldToScreenPoint(camera, position8);
    Vector3 vector8 = WorldToScreenPoint(camera, position9);

    if (vector.z > 0 && vector2.z > 0 && vector3.z > 0 && vector4.z > 0 && vector5.z > 0 && vector6.z > 0 && vector7.z > 0 && vector8.z > 0 )
    {
        esp.DrawLine(espColor, 2, Vector2((glWidth -(glWidth -vector.x)), (glHeight - vector.y)), Vector2((glWidth - (glWidth - vector2.x)), (glHeight - vector2.y)));
        esp.DrawLine(espColor, 2, Vector2((glWidth -(glWidth -vector3.x)), (glHeight - vector3.y)), Vector2((glWidth - (glWidth - vector2.x)), (glHeight - vector2.y)));
        esp.DrawLine(espColor, 2, Vector2((glWidth -(glWidth -vector.x)), (glHeight - vector.y)), Vector2((glWidth - (glWidth - vector4.x)), (glHeight - vector4.y)));
        esp.DrawLine(espColor, 2, Vector2((glWidth -(glWidth -vector4.x)), (glHeight - vector4.y)), Vector2((glWidth - (glWidth - vector3.x)), (glHeight - vector3.y)));
        
        esp.DrawLine(espColor, 2, Vector2((glWidth -(glWidth -vector5.x)), (glHeight - vector5.y)), Vector2((glWidth - (glWidth - vector6.x)), (glHeight - vector6.y)));
        esp.DrawLine(espColor, 2, Vector2((glWidth -(glWidth -vector7.x)), (glHeight - vector7.y)), Vector2((glWidth - (glWidth - vector6.x)), (glHeight - vector6.y)));
        esp.DrawLine(espColor, 2, Vector2((glWidth -(glWidth -vector5.x)), (glHeight - vector5.y)), Vector2((glWidth - (glWidth - vector8.x)), (glHeight - vector8.y)));
        esp.DrawLine(espColor, 2, Vector2((glWidth -(glWidth -vector8.x)), (glHeight - vector8.y)), Vector2((glWidth - (glWidth - vector7.x)), (glHeight - vector7.y)));
 
        esp.DrawLine(espColor, 2, Vector2((glWidth -(glWidth -vector.x)), (glHeight - vector.y)), Vector2((glWidth - (glWidth - vector5.x)), (glHeight - vector5.y)));
        esp.DrawLine(espColor, 2, Vector2((glWidth -(glWidth -vector2.x)), (glHeight - vector2.y)), Vector2((glWidth - (glWidth - vector6.x)), (glHeight - vector6.y)));
        esp.DrawLine(espColor, 2, Vector2((glWidth -(glWidth -vector3.x)), (glHeight - vector3.y)), Vector2((glWidth - (glWidth - vector7.x)), (glHeight - vector7.y)));
        esp.DrawLine(espColor, 2, Vector2((glWidth -(glWidth -vector4.x)), (glHeight - vector4.y)), Vector2((glWidth - (glWidth - vector8.x)), (glHeight - vector8.y)));
    }
}
bool inR = false;
bool inG = false;
bool inB = false;



Color Ka = Color(255,255,255,255);
Color Dis = Color(255,0,0,255);



bool R;
bool G;
bool B;

Color getColor() {
    return Ka;
}


void DrawESP(NepEsp esp, int screenWidth, int screenHeight) {
    esp.DrawText(Color::White(), "t.me/RealWanteD_Cheats", Vector2(screenWidth / 2, screenHeight/1.02), 25);
    
void *player = playerlate;
if(ESP && player != nullptr) {
    
{
    int gw = get_screen_width(), gh = get_screen_height();
    if (gw > 0 && gh > 0) {
        gScaleX = (float)screenWidth / (float)gw;
        gScaleY = (float)screenHeight / (float)gh;
    }
}
std::vector<void *> snap = espManager->snapshot();
    // Count (drawn once, even if no enemy is on screen)
    if (EspObject) {
        int enemyCount = 0;
        for (size_t k = 0; k < snap.size(); k++) {
            if (snap[k] != nullptr && PlayerAlive(snap[k])) enemyCount++;
        }
        int left = getEnemiesLeft();
        if (left >= 0) enemyCount = left;
        std::string enemyCountStr = "Enemies: " + std::to_string(enemyCount);
        esp.DrawText(Color::Red(), enemyCountStr.c_str(), Vector2(screenWidth / 2, screenHeight / 15), 35.0f);
    }
if (snap.empty()) {
  return;
    }
    for (size_t i = 0; i < snap.size(); i++) {
    void *Cam = get_camera();
    if(Cam != nullptr) {
     
        void *Player = snap[i];
        
        if (Player != nullptr) {
            if(PlayerAlive(Player)) {
            Color clr;
            clr.a = 255;
            Rect rect;
            Vector3 PlayerPos = GetPlayerLocation(Player);
            Vector3 PosNew = {0.f, 0.f, 0.f};
            PosNew = WorldToScreenPoint(Cam, PlayerPos);
            if (PosNew.z < 1.f) continue;
            // skip enemies outside the visible screen (no stray lines/boxes at the edges)
            {
                float sy = screenHeight - PosNew.y;
                if (PosNew.x < 0.f || PosNew.x > (float)screenWidth ||
                    sy < 0.f || sy > (float)screenHeight) continue;
            }
            
            
            
           
            Vector3 Origin;
            Origin = PlayerPos;
            Origin.y += 0.7f;
            float posnum = 10 * 0.1;
            float posnum2 = 10 * 0.1;
            Origin.y += posnum;
            Vector3 BoxPosNew = {0.f, 0.f, 0.f};
            BoxPosNew = WorldToScreenPoint(Cam, Origin);
            float Hight =
            abs(BoxPosNew.y - PosNew.y) * (posnum2 / posnum), Width = Hight * 2.0f;
            rect = Rect(BoxPosNew.x - Width / 2.f, screenHeight - BoxPosNew.y, Width, Hight);
            int MaxHealth = GetMaxHealth(Player);
            int CurHealth = GetPlayerHealth(Player);
            int max;
            max = 255;
            int max2;
            max2 = 255;
            int max3;
            max3 = 255;
            Color espColor = Color(rand()%max, rand()%max2, rand()%max3, 255);
                
                
                // Line
                int lineOffset = 10;
                Vector2 DrawFrom = Vector2(screenWidth / 2, screenHeight / 15);
                Vector2 DrawTo = Vector2((screenWidth - (screenWidth - BoxPosNew.x)) + 5, screenHeight - BoxPosNew.y - lineOffset);

                if (ESPLine) {
                esp.DrawLine(getColor(), 4, DrawFrom, DrawTo);
                }
                
               if(ESPBox) {
               esp.DrawBox(Color::Red(), 4, rect);
               }
               
               // 3d box
               if(ESPBox3d) {
               Draw3dBox(esp, getColor(), PlayerPos, Cam, screenHeight, screenWidth);
               }
            
            
               
                           if(ESPHealth) {
                esp.DrawHorizontalHealthBar(Vector2((screenWidth - (screenWidth - PosNew.x)), (screenHeight - PosNew.y - 17.0f)), 50, MaxHealth, CurHealth);
            }
               
               // Distance
               if(ESPDistance) {
               int distance = (int) Vector2::Distance(Vector2(screenWidth , screenHeight), Vector2(BoxPosNew.x, BoxPosNew.y));
               std::string distances;
               distances = float_to_string(distance / 100);
               esp.DrawText(getColor(),  distances.c_str(), Vector2((screenWidth - (screenWidth - BoxPosNew.x)), (screenHeight - BoxPosNew.y - 40)), 20.0f);
             //esp.DrawText(getColor(), displayString.c_str(), Vector2(screenWidth - (screenWidth - BoxPosNew.x), screenHeight - BoxPosNew.y - 40), 20.0f);
               }
               
               } else {
               espManager->removeEnemyGivenObject(Player);
               }
            }
         }
      }
   }
}

extern "C"
JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_DrawOn(JNIEnv *env, jclass type, jobject espView, jobject canvas) {
                es = NepEsp(env, espView, canvas);
    if (es.isValid()){
        DrawESP(es, es.getWidth(), es.getHeight());
    }
    
 }

void *enemyPlayer = NULL;
void (*update)(void *enemy);
void _update(void *enemy) {
    if (enemy != NULL) {
        update(enemy);
        get_Player(enemy);              // playerlate != nullptr
        if (ESP) espManager->tryAddEnemy(enemy);
    }
}

void *(*destroyP)(void *enemy);
void *_destroy(void *enemy) {
    if (enemy != NULL) espManager->removeEnemyGivenObject(enemy);
    return destroyP(enemy);
}
