// ======================== SMOOTH MOVEMENT ADDITIONS START ========================
#include <iostream>
#include <string>
#include "Player.h"
using namespace std;



Player::Player(Vector2& s){
    Sprite_sheet = LoadTexture("assets/Player_Animation_Sheet.png");
    Health = 100;
    Power = 20;

    Idle = {0,5,0,0.24,0.24};
    Attack = {0,7,0,0.15,0.15};
    Death = {0,7,0,0.16,0.16};
    MoveHorizontal = {0,7,0,0.07,0.07};
    MoveVertical = {0,3,0,0.07,0.07};

    currentAnim = &Idle;
    currentRow = 4;
    isActionPlaying = false;
    isFlipped = false;

    GridPosition = s;
    Position = {s.x * 32, s.y * 32};
    TargetPosition = Position;
    moveSpeed = 100.0f;
    isMoving = false;
}

Player::~Player(){
    UnloadTexture(Sprite_sheet);
}

Rectangle Player::GetAnimationFrame(int Frame, int FrameRow, int FrameWidth, int FrameHeight){

    int xPos = Frame*FrameWidth;
    int YPos = FrameRow*FrameHeight;

    return {(float)xPos,(float)YPos,(float)32,(float)32};
}

void Player::Update_Animation(Animation* Anim){
    float dt = GetFrameTime();
    Anim->DurationLeft -= dt;

    if(Anim->DurationLeft<=0){
        Anim->DurationLeft = Anim->Speed;
        Anim->CurrentFrame++;

        if(Anim->CurrentFrame>Anim->LastFrame){
            Anim->CurrentFrame = Anim->FirstFrame;

            if(currentAnim==&Attack||currentAnim==&Death){
                isActionPlaying = false;
            }
        }
    }

}

void Player::SetAnimation(Animation& anim, int r){
    if(currentAnim != &anim||currentRow!=r){

        currentAnim = &anim;
        currentRow = r;

        currentAnim->CurrentFrame = anim.FirstFrame;
        currentAnim->DurationLeft = anim.Speed;
    }
}


void Player::HandleInput(Map* l){
    if (isActionPlaying || isMoving) return;

    if (IsKeyPressed(KEY_SPACE)) {
        SetAnimation(getAnim("attack"), 8);
        setActionStatus(true);
    }
    else if (IsKeyPressed(KEY_K)) {
        SetAnimation(getAnim("death"), 7);
        setActionStatus(true);
    }
    else if (IsKeyDown(KEY_A)) {
        SetAnimation(getAnim("moveH"), 3);
        setFlip(true);
        Move('L',l);
    }
    else if (IsKeyDown(KEY_D)) {
        SetAnimation(getAnim("moveH"), 3);
        setFlip(false);
        Move('R',l);
    }
    else if (IsKeyDown(KEY_W)){
        SetAnimation(getAnim("moveV"), 2);
        setFlip(false);
        Move('U',l);
    }
    else if(IsKeyDown(KEY_S)){
        SetAnimation(getAnim("moveV"), 2);
        setFlip(false);
        Move('D',l);
    }
    else {
        SetAnimation(getAnim("idle"), 4);
    }
}

void Player::Move(char n, Map* l){
    Vector2 offset = { 0, 0 };
    switch (n) {
        case 'L': offset = { -1, 0 }; break;
        case 'R': offset = { 1, 0 }; break;
        case 'U': offset = { 0, -1 }; break;
        case 'D': offset = { 0, 1 }; break;
    }

    Vector2 next = { GridPosition.x + offset.x, GridPosition.y + offset.y };
    if (l->isWalkable((int)next.x, (int)next.y)) {
        GridPosition = next;
        TargetPosition = { next.x * 32, next.y * 32 };
        isMoving = true;
    }
}


void Player::Update(float dt) {
    if (isMoving) {
        if (Position.x < TargetPosition.x) {
            Position.x += moveSpeed * dt;
            if (Position.x > TargetPosition.x) Position.x = TargetPosition.x;
        }
        else if (Position.x > TargetPosition.x) {
            Position.x -= moveSpeed * dt;
            if (Position.x < TargetPosition.x) Position.x = TargetPosition.x;
        }

        if (Position.y < TargetPosition.y) {
            Position.y += moveSpeed * dt;
            if (Position.y > TargetPosition.y) Position.y = TargetPosition.y;
        }
        else if (Position.y > TargetPosition.y) {
            Position.y -= moveSpeed * dt;
            if (Position.y < TargetPosition.y) Position.y = TargetPosition.y;
        }

        if (Position.x == TargetPosition.x && Position.y == TargetPosition.y) {
            isMoving = false;
        }
    }

    Update_Animation(currentAnim);
}

void Player::Draw(){
    Rectangle PlayerFrame = GetAnimationFrame(getCurrentAnim()->CurrentFrame,getCurRow(),32, 32);

    if (!getFlip()) {
        DrawTexturePro(Sprite_sheet,PlayerFrame,{Position.x, Position.y, 32, 32 },{ 0.0f, 0.0f }, 0.0f, WHITE);
    }else{
        PlayerFrame.width = -(PlayerFrame.width);
        DrawTexturePro(Sprite_sheet,PlayerFrame,{Position.x, Position.y, 32, 32},{0.0f,0.0f},0.0f, WHITE);
    }
}

Animation& Player::getAnim(string A){

    if(A=="idle"){
        return this->Idle;
    }else if(A=="attack"){
        return this->Attack;
    }else if(A=="death"){
        return this->Death;
    }else if(A=="moveH"){
        return this->MoveHorizontal;
    }else{
        return this->MoveVertical;
    }
}

Animation* Player::getCurrentAnim(){
    return currentAnim;
}

bool Player::getFlip(){
    return isFlipped;
}

int Player::getCurRow(){
    return currentRow;
}

bool Player::getActionStatus(){
    return isActionPlaying;
}

void Player::setActionStatus(bool x){
    isActionPlaying = x;
}

void Player::setFlip(bool x){
    isFlipped = x;
}