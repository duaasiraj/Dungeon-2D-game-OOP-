#include <fstream>
#include <iostream>
#include "HighScoreManager.h"

using namespace std;

HighScoreManager::HighScoreManager(const string& fileName){

    try{
        ifstream FileIn(fileName);
        if(!FileIn){throw("ERROR: File not found");}
        else{FileIn>>highScore;
        FileIn.close();
        filename = fileName;}
    }
    catch(char const* msg){
        cout<<msg<<endl;
    }
    
}

int HighScoreManager::GetHighScore(){
    return highScore;
}

bool HighScoreManager::Check(int score){

    if(score>highScore){
        highScore = score;

        try{
        ofstream FileOut(filename);
        if(!FileOut){throw("ERROR: File not found");
        
        }
        else{
        
        FileOut<<highScore;

        FileOut.close();
        return true;
        
        }
        
    }
        catch(char const* msg){
            cout<<msg<<endl;
            return false;
        }
        
    }else{
        return false;
    }
}