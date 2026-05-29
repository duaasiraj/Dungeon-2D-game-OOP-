#include "TextManager.h"
#include <iostream>
#include <raylib.h>
#include <string>

using namespace std;


TextManager::TextManager(){ 
    
    
    timer = 0.0f;
    fontSize = 14;
    color = GREEN;
    pos = {0,0}; 
    msg = "";

    }


void TextManager::Create(const string& m, Vector2 p, Color clr, int size, float duration) {
    msg = m;
    pos = p;
    color = clr;
    fontSize = size;
    timer = duration;
}

void TextManager::Update() {
    if (timer>0.0f) {
        timer -= GetFrameTime();
    }
}

void TextManager::Draw(){
    if (timer > 0.0f) {
        DrawText(msg.c_str(), (int)pos.x, (int)pos.y, fontSize, color);
    }
}

bool TextManager::IsActive(){
    return timer > 0.0f;
}
