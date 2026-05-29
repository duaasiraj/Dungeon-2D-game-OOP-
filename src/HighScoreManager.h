#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#include <string>
#include <iostream>
using namespace std;

class HighScoreManager{

    string filename;
    int highScore;

public:
    HighScoreManager(const string& fileName);
    int GetHighScore();
    bool Check(int score);
};

#endif
