#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include "raylib.h"
#include "Map.h"
#include "Animation.h"
using namespace std;

class Player
{

        int Health;
        int Power;
        int Score;

        Animation Idle;
        Animation Attack;
        Animation Death;
        Animation MoveHorizontal;
        Animation MoveVertical;
        Animation *currentAnim;
        bool isActionPlaying;
        bool isFlipped;
        int currentRow;

        // ======================== SMOOTH MOVEMENT ADDITIONS START ========================
        Vector2 GridPosition;
        Vector2 TargetPosition;
        float moveSpeed;
        bool isMoving;
        // ======================== SMOOTH MOVEMENT ADDITIONS END ========================

public:
        Vector2 Position;
        Texture2D Sprite_sheet;

        Player(Vector2 &s);
        ~Player();

        void TakeDamage(int amount);
        bool IsDead();

        void Update(float dt);
        void Update_Animation(Animation *Anim);
        Rectangle GetAnimationFrame(int Frame, int FrameRow, int FrameWidth, int FrameHeight);
        void SetAnimation(Animation &anim, int r);

        Animation &getAnim(string A);
        Animation *getCurrentAnim();
        bool getFlip();
        int getCurRow();
        bool getActionStatus();
        void setActionStatus(bool x);
        void setFlip(bool x);

        void Draw();
        void HandleInput(Map *l);

        void Move(char n, Map *l);

        void setPos(Vector2& s);

        void ResetToSpawn(Vector2 spawn);

        Vector2& getGridPos();

        void setHealth(int h);
        void setScore(int s);
        void setPower(int p);

        int getHealth();
        int getScore();
        int getPower();

        void Reset();
};

#endif