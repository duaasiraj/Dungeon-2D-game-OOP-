#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include <string>
#include "Animation.h"

class Enemy
{
protected:
    int Health;
    bool isFlipped;

    Animation Idle;
    Animation Walk;
    Animation Attack;
    Animation Die;
    Animation *CurrentAnim;

    Texture2D SpriteSheet;

    void SetAnimation(Animation &anim, int row);
    void UpdateAnimation(Animation *anim);

    int CurrentRow;

public:
    Vector2 Position;
    virtual ~Enemy() {}
    bool isActionPlaying;

    virtual void Update(float dt) = 0;
    virtual void Draw() = 0;
    virtual void TakeDamage(int amount) = 0;
    virtual bool IsDead() = 0;
};

#endif
