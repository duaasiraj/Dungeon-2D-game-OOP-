#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H


#include <raylib.h>
#include <iostream>
#include <string>
using namespace std;

class TextManager{

    string msg;
    Vector2 pos;;
    float timer;
    Color color;
    int fontSize;

public:
    TextManager();

    void Create(const string& m, Vector2 p, Color clr, int size = 14, float duration = 2.0f);
    void Update();
    void Draw();
    bool IsActive();

};

#endif