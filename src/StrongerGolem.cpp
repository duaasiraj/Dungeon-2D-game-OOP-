#include "golem.h"
#include "raymath.h"
#include "raylib.h"
#include "Map.h"
#include "StrongerGolem.h"


StrongerGolem::StrongerGolem(Vector2 position):Golem(position)
{
    Position.x= position.x*32;
    Position.y= position.y*32;
    Health = 150;

    // Load separate sprite sheets for each animation
    IdleSheet = LoadTexture("assets/EnemyAssets/Idle2.png");
    WalkSheet = LoadTexture("assets/EnemyAssets/Walk2.png");
    AttackSheet = LoadTexture("assets/EnemyAssets/Attack2.png");
    DieSheet = LoadTexture("assets/EnemyAssets/Death2.png");

    Idle = {0, 3, 0, 0.2f, 0.2f};      
    Walk = {0, 5, 0, 0.1f, 0.1f};     
    Attack = {0, 3, 0, 0.1f, 0.1f};   
    Die = {0, 6, 0, 0.15f, 0.15f};   

    map = nullptr;
    attackDone = false;
    
}

StrongerGolem::~StrongerGolem()
{
    UnloadTexture(IdleSheet);
    UnloadTexture(WalkSheet);
    UnloadTexture(AttackSheet);
    UnloadTexture(DieSheet);
}

void StrongerGolem::TakeDamage(int amount)
{
    if (State == "die") {
        return;
    }
    HitsTaken++;
    // Handle damage
    if (HitsTaken >= 5) {
        State = "die";
        SetAnimation(Die, 0);
        isActionPlaying = false;
    } else {

        SetAnimation(Attack, 0); // Use attack anim as hit reaction
        State = "attack";
        isActionPlaying = true;
        attackDone = false;
    }
}