#include "Map.h"
#include <raylib.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

Rectangle Map::getTileRec(int TileID){
    int xCoord = (TileID%tilesPerRow)*tileSize;
    int yCoord = (TileID/tilesPerRow)*tileSize;

    Rectangle temp = {(float)xCoord, (float)yCoord,(float)tileSize,(float)tileSize};
    return temp;
}

Map::Map(){}

Map::Map(const string& filename,int w, int f, int e,char const* tileFile){
    tileset = LoadTexture(tileFile);
    exitTexture = LoadTexture("assets/Map_Assets/exit.png");
    tileSize = 32;
    tilesetHeight = tileset.height;
    tilesetWidth = tileset.width;
    tilesPerRow = tilesetWidth/tileSize;
    currEnemyCount = 0;
    maxEnemyCount = 3;
    wallID = w;
    floorID = f;
    exitID = e;

    loadFile(filename);

    
    
}

Map::~Map(){
    UnloadTexture(tileset);
    UnloadTexture(exitTexture);

    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 30; x++) {
            if (collectables[y][x]) {
            delete collectables[y][x];
            collectables[y][x] = nullptr;
            }
        }
    }
    
}

void Map::loadFile(const string& filename){

    ifstream MapFile(filename);
    if(!MapFile.is_open()){
        cout<<"Failed to load file"<<endl;
        return;
    }

    int y = 0;
    string line;

    while(getline(MapFile,line)&&y<20){
        for(int x=0;x<line.length()&&x<30;x++){
            char c = line[x];

            switch(c){

                case '#':
                    if(y==0 && x==0){                                                  //Top-left corner tile
                        CompleteMap[y][x] = {false,false,false,false,false,wallID}; 
                    }else if(y==0 && x==29){                                           //Top-right corner tile
                        CompleteMap[y][x] = {false,false,false,false,false,wallID};
                    }else if(y==19 && x==0){                                           //Bottom-left corner tile
                        CompleteMap[y][x] = {false,false,false,false,false,wallID};
                    }else if(y==19 && x==29){                                          //Bottom-right corner tile
                        CompleteMap[y][x] = {false,false,false,false,false,wallID};
                    }else if(x==0){                                                    //Left side wall
                        
                        CompleteMap[y][x] = {false,false,false,false,false,wallID};

                    }else if(x==29){                                                   //Right side wall
                        
                        
                        CompleteMap[y][x] = {false,false,false,false,false,wallID};

                    }else if(y==0){                                                    //Top of map
                        CompleteMap[y][x] = {false,false,false,false,false,wallID};
                    }else if(y==19){                                                   //Bottom of map
                        CompleteMap[y][x] = {false,false,false,false,false,wallID};
                    }else{                                                             //All walls inside the map
                        
                        CompleteMap[y][x] = {false,false,false,false,false,wallID};
                    }
                    
                    break;
                
                case '.':
                    CompleteMap[y][x] = {true,false,false,false,false,floorID};
                    break;

                case 'P':
                    CompleteMap[y][x] = {true,false,false,false,true,floorID};
                    Spawn = {(float)x,(float)y};
                    break;

                case 'E':
                    CompleteMap[y][x] = {true,true,false,false,false,exitID};
                    Exit = {(float)x,(float)y};
                    break;
                    

                case '$':
                    CompleteMap[y][x] = {true,false,false,true,false,floorID};
                    collectables[y][x] = new Collectable("Score",10);
                    collectables[y][x]->SetMapPos({(float)x * 32, (float)y * 32});
                break;

                case 'H':
                    CompleteMap[y][x] = {true,false,false,true,false,floorID};
                    collectables[y][x] = new Collectable("Health",10);
                    collectables[y][x]->SetMapPos({(float)x * 32, (float)y * 32});
                break;

                case 'X':
                    CompleteMap[y][x] = {true,false,true,false,false,floorID};
                    if (currEnemyCount < maxEnemyCount) {
                        EnemySpawn[currEnemyCount] = {(float)x,(float)y};
                        currEnemyCount++;
                        cout<<"Enemy Spawned at: "<<x<<","<<y<<endl;
                    }
                break;

                default:
                    CompleteMap[y][x] = {true,false,false,false,false,floorID};
                
            }
        }
        y++;
    }

    MapFile.close();

}

void Map::setTile(int x, int y, bool isW, bool isE, bool isX, bool isC, bool isS, int ID){
    if(y<20&&y>=0&&x>0&&x<30){
        CompleteMap[y][x]={isW,isE,isX,isC,isS,ID};
    }
}

bool Map::isWalkable(int x, int y){
    if(y<20&&y>=0&&x>0&&x<30){
        return CompleteMap[y][x].walkable;
    }else{
        return false;
    }
}

void Map::Draw()
{
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 30; x++) {
            Vector2 pos = {(float)x * tileSize, (float)y * tileSize};
            Rectangle src = getTileRec(CompleteMap[y][x].tileID);

            if (CompleteMap[y][x].tileID == exitID) {
                DrawTextureRec(exitTexture, {0, 0, 32, 32}, pos, WHITE);
            } else {
                DrawTextureRec(tileset, src, pos, WHITE);
            }

            if(CompleteMap[y][x].collectable && collectables[y][x] != nullptr&&!collectables[y][x]->IsCollected()){
                collectables[y][x]->Draw();
                collectables[y][x]->Update();
            }
        }
    }

    
}

Vector2& Map::getEnemySpawn(int index){
    if(index<3){
        return EnemySpawn[index];
    }
}


Vector2& Map::getSpawn(){
    return Spawn;
}

Vector2& Map::getExit(){
    return Exit;
}

Tile& Map::getTile(int x, int y) {
    return CompleteMap[y][x];
}

Collectable* (*Map::getcollectables())[30] {
    return collectables;
}

Tile (&Map::getMap())[20][30] {
    return CompleteMap;
}





void Map::Reset(const string& filename, int w, int f, int e, const char* tileFile) {
    // Clean existing collectables
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 30; x++) {
            if (collectables[y][x]) {
                delete collectables[y][x];
                collectables[y][x] = nullptr;
            }
        }
    }

    // Reload tile texture
    if (tileset.id > 0) UnloadTexture(tileset);
    if (exitTexture.id > 0) UnloadTexture(exitTexture);

    tileset = LoadTexture(tileFile);
    exitTexture = LoadTexture("assets/Map_Assets/exit.png");
    tileSize = 32;
    tilesetHeight = tileset.height;
    tilesetWidth = tileset.width;
    tilesPerRow = tilesetWidth / tileSize;

    wallID = w;
    floorID = f;
    exitID = e;

    loadFile(filename);
}

