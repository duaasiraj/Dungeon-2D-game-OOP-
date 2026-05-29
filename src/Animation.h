
#ifndef ANIMATION_H
#define ANIMATION_H

struct Animation
{
    int FirstFrame;
    int LastFrame;
    int CurrentFrame;
    float Speed;
    float DurationLeft;
};

#endif