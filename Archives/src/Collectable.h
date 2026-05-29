#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include <iostream>
#include <string>
#include "raylib.h"
#include "Animation.h"
using namespace std;

class Collectable{

    string type;
    int value;
    bool isCollected;
    

    Animation Idle;
    Animation Collected;
    Animation *currentAnim;
    int currentRow;

    Texture2D Idlesheet;
    Texture2D Effectsheet;

    Vector2 MapPos;

    public:
        
        Collectable();
        Collectable(const char* t, int v);
        ~Collectable();

        void Update_Animation(Animation *Anim);
        Rectangle GetAnimationFrame(int Frame, int FrameWidth, int FrameHeight);
        void SetAnimation(Animation &anim);

        Animation *getCurrentAnim();
        void Draw();
        int getCurRow();


        int getValue();
        bool getCollect();

        bool IsCollected();
        void SetMapPos(Vector2 pos);
        void Update();
        void Collect();

        string getType();

};    

#endif