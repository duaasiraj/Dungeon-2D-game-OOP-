#include "Enemy.h"

void Enemy::SetAnimation(Animation &anim, int row)
{
    if (CurrentAnim != &anim || CurrentRow != row)
    {
        CurrentAnim = &anim;
        CurrentRow = row;
        CurrentAnim->CurrentFrame = anim.FirstFrame;
        CurrentAnim->DurationLeft = anim.Speed;
    }
}

void Enemy::UpdateAnimation(Animation *anim)
{
    float dt = GetFrameTime();
    anim->DurationLeft -= dt;
    if (anim->DurationLeft <= 0)
    {
        anim->DurationLeft = anim->Speed;
        anim->CurrentFrame++;

        if (anim->CurrentFrame > anim->LastFrame)
        {
            anim->CurrentFrame = anim->FirstFrame;
            if (CurrentAnim == &Attack || CurrentAnim == &Die)
            {
                isActionPlaying = false;
            }
        }
    }
}
