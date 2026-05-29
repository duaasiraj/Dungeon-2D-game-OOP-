#include "Collectable.h"
#include <raylib.h>
#include <cstring>

Collectable::Collectable() {}

Collectable::Collectable(const char* t, int v)
{
    value = v;
    isCollected = false;

    if (strcmp(t, "Score") == 0) {
        Idlesheet = LoadTexture("assets/Collectable_Assets/score_sprite.png");
        Effectsheet = LoadTexture("assets/Collectable_Assets/score_effect.png");
        Idle = {0, 13, 0, 0.1f, 0.1f};       
        Collected = {0, 7, 0, 0.06f, 0.06f};    
    } else {
        Idlesheet = LoadTexture("assets/Collectable_Assets/heart_rotate.png");
        Effectsheet = LoadTexture("assets/Collectable_Assets/health_effect.png");
        Idle = {0, 11, 0, 0.05f, 0.05f};        
        Collected = {0, 4, 0, 0.06f, 0.06f};    
    }

    currentAnim = &Idle;

    type = t;
}

Collectable::~Collectable()
{
    UnloadTexture(Idlesheet);
    UnloadTexture(Effectsheet);
}

Rectangle Collectable::GetAnimationFrame(int frame, int frameWidth, int frameHeight)
{
    int x = frame * frameWidth;
    return {(float)x, 0.0f, (float)frameWidth, (float)frameHeight};
}

void Collectable::Update_Animation(Animation* anim)
{
    float dt = GetFrameTime();
    anim->DurationLeft -= dt;

    if (anim->DurationLeft <= 0) {
        anim->DurationLeft = anim->Speed;
        anim->CurrentFrame++;

        if (anim->CurrentFrame > anim->LastFrame) {
            if (currentAnim == &Collected)
            {
                isCollected = true;
                return;  
            }
            anim->CurrentFrame = anim->FirstFrame;
        }
    }
}

void Collectable::SetAnimation(Animation& anim)
{
    if (currentAnim != &anim) {
        currentAnim = &anim;
        currentAnim->CurrentFrame = anim.FirstFrame;
        currentAnim->DurationLeft = anim.Speed;
    }
}

void Collectable::Collect()
{
    SetAnimation(Collected);
}

void Collectable::Update()
{
    Update_Animation(currentAnim);
}

void Collectable::Draw()
{
    if (isCollected && currentAnim != &Collected)
        return;

    Rectangle src = GetAnimationFrame(currentAnim->CurrentFrame, 32, 32);
    Texture2D* tex = (currentAnim == &Collected) ? &Effectsheet : &Idlesheet;

    DrawTextureRec(*tex, src, MapPos, WHITE);
}

bool Collectable::IsCollected()
{
    return isCollected;
}

void Collectable::SetMapPos(Vector2 pos)
{
    MapPos = pos;
}

string Collectable::getType(){
    return type;
}

int Collectable::getValue(){
    return value;
}