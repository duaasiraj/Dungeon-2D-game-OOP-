#include "button.h"
#include <raylib.h>
#include <iostream>
using namespace std;

Button::Button(){}

Button::Button(const char*  file, Vector2& p) {
    
    pos = p;
    scale = 0.5f;
    image = LoadTexture(file);
    pos = {(GetScreenWidth() - image.width * scale) / 2,(GetScreenHeight() - image.height * scale) / 2};
    
    
}

Button::~Button() {
    UnloadTexture(image);
}

void Button::Draw() {
    Rectangle src = { 0, 0, (float)image.width, (float)image.height };
    Rectangle dest = { pos.x, pos.y, (float)image.width*scale, (float)image.height*scale };
    Vector2 origin = { 0, 0 };

    DrawTexturePro(image, src, dest, origin, 0.0f, WHITE);
}

bool Button::isClicked(){
    Vector2 mouse = GetMousePosition();
    Rectangle bounds = { pos.x, pos.y, (float)image.width*scale, (float)image.height*scale };
    return CheckCollisionPointRec(mouse, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}
