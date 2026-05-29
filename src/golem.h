#ifndef GOLEM_H
#define GOLEM_H

#include "Enemy.h"
#include <string>
#include "Map.h"

class Golem : public Enemy
{   protected:
    float moveSpeed;

    Vector2 *PlayerPosition;
    float WanderTimer;
    int HitsTaken;


    float   stateChangeCooldown;
   float  directionChangeCooldown;
    // Separate sprite sheets for each animation
    Texture2D IdleSheet;
    Texture2D WalkSheet;
    Texture2D AttackSheet;
    Texture2D DieSheet;

public:
    Golem(Vector2 position);
    ~Golem();
    Map *map; // Pointer to the map
    void SetMap(Map *m);
    bool attackDone;
    std::string State;
    int patrolStartTileX;
    int patrolEndTileX;
    bool CanMoveTo(float nextX, float nextY);
    void Update(float dt) override;
    void Draw() override;
    void TakeDamage(int amount) override;
    bool IsDead() override;

    void SetTarget(Vector2 *playerPos);

    void Reset(Vector2 newPos);

private:
    void AI_Behavior(float dt);
};

#endif

