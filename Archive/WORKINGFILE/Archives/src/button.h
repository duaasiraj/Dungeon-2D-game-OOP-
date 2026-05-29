#ifndef BUTTON_H
#define BUTTON_H



#include <raylib.h>
#include <iostream>
#include <string>
using namespace std;

class Button{

    Texture2D image;
    Vector2 pos;
    float scale;

    public:
        Button();
        ~Button();
        Button(const char* file, Vector2& p);

        void Draw();
        bool isClicked();
};



#endif