#include "golem.h"
#include "raymath.h"
#include "raylib.h"
#include "Map.h"


Golem::Golem(Vector2 position)
{
    Position.x= position.x*32;
    Position.y= position.y*32;
    Health = 150;

    // Load separate sprite sheets for each animation
    IdleSheet = LoadTexture("assets/EnemyAssets/Idle.png");
    WalkSheet = LoadTexture("assets/EnemyAssets/Walk.png");
    AttackSheet = LoadTexture("assets/EnemyAssets/Attack.png");
    DieSheet = LoadTexture("assets/EnemyAssets/Death.png");

    Idle = {0, 3, 0, 0.2f, 0.2f};      
    Walk = {0, 5, 0, 0.1f, 0.1f};     
    Attack = {0, 5, 0, 0.1f, 0.1f};   
    Die = {0, 6, 0, 0.15f, 0.15f};   
    CurrentAnim = &Idle;
    CurrentRow = 0;
    isActionPlaying = false;
    isFlipped = false;
    moveSpeed = 20;
    State = "idle";
        stateChangeCooldown = 0.0f;
    directionChangeCooldown = 0.0f;
    PlayerPosition = nullptr;
    HitsTaken = 0;

    map = nullptr;
    attackDone = false;
    
}


Golem::~Golem()
{
    UnloadTexture(IdleSheet);
    UnloadTexture(WalkSheet);
    UnloadTexture(AttackSheet);
    UnloadTexture(DieSheet);
}

void Golem::SetTarget(Vector2 *playerPos)
{
    PlayerPosition = playerPos;
}

void Golem::SetMap(Map *m)
{
    map = m;
}

bool Golem::CanMoveTo(float nextX, float nextY)
{
    if (!map) return false;
    
    // Get destination position tiles
    int destLeftTile = (int)(nextX / 32);
    int destRightTile = (int)((nextX + 31) / 32);
    int destTopTile = (int)(nextY / 32);
    int destBottomTile = (int)((nextY + 31) / 32);
    
    // Ensure we're checking valid tile positions
    destLeftTile = Clamp(destLeftTile, 0, 29);
    destRightTile = Clamp(destRightTile, 0, 29);
    destTopTile = Clamp(destTopTile, 0, 19);
    destBottomTile = Clamp(destBottomTile, 0, 19);
    
    // Determine direction of movement
    bool movingRight = nextX > Position.x;
    bool movingLeft = nextX < Position.x;
    bool movingDown = nextY > Position.y;
    bool movingUp = nextY < Position.y;
    
    // Default to checking all tiles
    bool canMove = true;
    
    // If moving horizontally (left or right)
    if (movingRight) {
        // Check right edge tiles
        canMove = canMove && map->isWalkable(destRightTile, destTopTile) && 
                              map->isWalkable(destRightTile, destBottomTile);
    } 
    else if (movingLeft) {
        // Check left edge tiles
        canMove = canMove && map->isWalkable(destLeftTile, destTopTile) && 
                              map->isWalkable(destLeftTile, destBottomTile);
    }
    
    // If moving vertically (up or down)
    if (movingDown) {
        // Check bottom edge tiles
        canMove = canMove && map->isWalkable(destLeftTile, destBottomTile) && 
                              map->isWalkable(destRightTile, destBottomTile);
    } 
    else if (movingUp) {
        // Check top edge tiles
        canMove = canMove && map->isWalkable(destLeftTile, destTopTile) && 
                              map->isWalkable(destRightTile, destTopTile);
    }
    
    // If not moving in either direction, check all corners
    if (!movingLeft && !movingRight && !movingUp && !movingDown) {
        canMove = map->isWalkable(destLeftTile, destTopTile) &&
                  map->isWalkable(destRightTile, destTopTile) &&
                  map->isWalkable(destLeftTile, destBottomTile) &&
                  map->isWalkable(destRightTile, destBottomTile);
    }
    
    return canMove;
}

void Golem::AI_Behavior(float dt)
{
    // Skip AI processing if dead or missing required components
    if (State == "die" || !PlayerPosition || !map) return;
    
    // Update cooldowns
    stateChangeCooldown -= dt;
    directionChangeCooldown -= dt;
    
    const float chaseRange = 350.0f;
    const float fasterChase=150.0f;  // Increased range for better detection
    const float attackRange = 40.0f;  // Slightly increased attack range
    
    // Calculate center points for distance calculation
    Vector2 playerCenter = {
        PlayerPosition->x + 16,
        PlayerPosition->y + 16
    };
    
    Vector2 golemCenter = {
        Position.x + 16,
        Position.y + 16
    };
    
    // Calculate distance between player and golem
    float distance = Vector2Distance(playerCenter, golemCenter);
    
    // ====== ATTACK BEHAVIOR ======
    if (distance <= attackRange) {
        // Face the player (but don't change direction too rapidly)
        if (directionChangeCooldown <= 0.0f) {
            isFlipped = playerCenter.x < golemCenter.x;
            directionChangeCooldown = 0.5f; // Half second cooldown for direction changes
        }
        
        // Start attack animation if not already attacking
        if (State != "attack" && !isActionPlaying && stateChangeCooldown <= 0.0f) {
            State = "attack";
            SetAnimation(Attack, 0);
            isActionPlaying = true;
            attackDone = false;
            stateChangeCooldown = 0.5f;
        }
        return;
    }
    
    // ====== CHASE BEHAVIOR ======
    if (distance <= chaseRange) {
        moveSpeed = 30; // Speed up when chasing
        // Only change to chase state if we're not in a cooldown period
        if (State != "chase" && stateChangeCooldown <= 0.0f) {
            State = "chase";
            SetAnimation(Walk, 0);
            stateChangeCooldown = 0.5f;
        }
        if (distance <= fasterChase){
            moveSpeed = 50; // Speed up when chasing
        }
        
        // Allow updating the direction even during chase, but with a cooldown
        if (directionChangeCooldown <= 0.0f) {
            isFlipped = playerCenter.x < golemCenter.x;
            directionChangeCooldown = 0.25f; // Quarter second cooldown for direction changes during chase
        }
        
        // Determine direction to move
        float dirX = 0;
        float dirY = 0;
        
        // Calculate movement vector toward player
        if (playerCenter.x < golemCenter.x) {
            dirX = -1;
        } else if (playerCenter.x > golemCenter.x) {
            dirX = 1;
        }
        
        if (playerCenter.y < golemCenter.y) {
            dirY = -1;
        } else if (playerCenter.y > golemCenter.y) {
            dirY = 1;
        }
        
        // Try horizontal and vertical movement independently to avoid getting stuck
        bool movedHorizontally = false;
        bool movedVertically = false;
        
        // Try horizontal movement first if there's significant horizontal distance
        if (fabsf(playerCenter.x - golemCenter.x) > 8) {
            float nextX = Position.x + dirX * moveSpeed * dt;
            if (CanMoveTo(nextX, Position.y)) {
                Position.x = nextX;
                movedHorizontally = true;
            }
        }
        
        // Try vertical movement if there's significant vertical distance
        if (fabsf(playerCenter.y - golemCenter.y) > 8) {
            float nextY = Position.y + dirY * moveSpeed * dt;
            if (CanMoveTo(Position.x, nextY)) {
                Position.y = nextY;
                movedVertically = true;
            }
        }
        
        // If stuck, try diagonal movement as a last resort
        if (!movedHorizontally && !movedVertically && (dirX != 0 && dirY != 0)) {
            float nextX = Position.x + dirX * moveSpeed * dt * 0.7f; 
            float nextY = Position.y + dirY * moveSpeed * dt * 0.7f;
            
            // Try to move diagonally
            if (CanMoveTo(nextX, nextY)) {
                Position.x = nextX;
                Position.y = nextY;
            }
        }
        
        return;
    }
    
}


void Golem::Update(float dt)
{
    // Skip updates if we're dead and the death animation is finished
    if (State == "die" && !isActionPlaying) {
        return;
    }
    
    // Update the current animation
    UpdateAnimation(CurrentAnim);
    
    // Check for attack frame to apply damage
    if (State == "attack" && CurrentAnim == &Attack) {
        if (CurrentAnim->CurrentFrame == CurrentAnim->LastFrame && !attackDone) {
            attackDone = true;

        }
        
        // When attack animation completes
        if (!isActionPlaying) {
            State = "idle";
            SetAnimation(Idle, 0);
        }
    }
    
    // Run AI behavior 
    AI_Behavior(dt);
    

}

void Golem::Draw()
{
    // Select the appropriate texture based on the current state
    Texture2D currentTexture;
    if (State == "die" && !isActionPlaying) {
    return;
}
    // Pick texture based on animation state
    if (CurrentAnim == &Attack)
        currentTexture = AttackSheet;
    else if (CurrentAnim == &Die)
        currentTexture = DieSheet;
    else if (CurrentAnim == &Walk)
        currentTexture = WalkSheet;
    else
        currentTexture = IdleSheet;
    
    // Define the source rectangle from the sprite sheet
    Rectangle sourceRect = {
        (float)(CurrentAnim->CurrentFrame * 32), 
        (float)(CurrentRow * 32),               
        32.0f,                                 
        32.0f                                  
    };
    
    // Define the destination rectangle on screen
    Rectangle destRect = {
        (Position.x),    
        (Position.y),     
        32.0f,         
        32.0f          
    };
    
    // Draw origin (top-left corner)
    Vector2 origin = {0, 0};
    
    // Handle flipping the sprite horizontally if needed
    if (isFlipped) {
        sourceRect.width = -sourceRect.width;
    }
    
    // Draw the sprite
    DrawTexturePro(currentTexture, sourceRect, destRect, origin, 0, WHITE);
    

}

void Golem::TakeDamage(int amount)
{
    // Skip if already dead
    if (State == "die") {
        return;
    }
    
    HitsTaken++;
    // Handle damage
    if (HitsTaken >= 3) {
        // Golem dies after 3 hits
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

bool Golem::IsDead()
{
    return (State == "die" && !isActionPlaying);
}

void Golem::Reset(Vector2 newPos){
    Position = newPos;
    Health = 5;
    State = "idle";
}