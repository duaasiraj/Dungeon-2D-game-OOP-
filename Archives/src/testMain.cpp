#include "Map.h"
#include "Player.h"
#include "button.h"
#include "Golem.h"
#include "TextManager.h"
#include "HighScoreManager.h"
#include <raylib.h>
#include <raymath.h>
#include <string>

Golem *globalGolem = nullptr; // define here
float cSCooldown = 0.0f;
float hSCooldown = 0.0f;
float pACooldown = 0.0f;
float gOCooldown = 0.0f;

bool gameStarted = false;
bool gameOver = false;
bool gOplayed = false;
float floatingTextTimer = 0.0f;
string floatingText = "";
Vector2 floatingTextPos = {0, 0};
int currEnemy = 0;
Enemy *enemies[3];


Map LoadLevel(const string &p, int w, int f, int e, const char *t)
{

    return Map(p, w, f, e, t);
}

int main()
{

    ///////////////////////////////// INITIAL SET UP STARTS ///////////////////////////////////////////////////////

    const int virtualWidth = GetScreenWidth();
    const int virtualHeight = GetScreenHeight();
    const int tileSize = 32;
    const int mapCols = 30;
    const int mapRows = 20;

    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(virtualWidth, virtualHeight, "2D Dungeon Explorer");
    InitAudioDevice();
    ToggleFullscreen();
    SetTargetFPS(60);

    Texture2D background = LoadTexture("assets/menu.png");
    Texture2D exitbackground = LoadTexture("assets/exit.png");
    Texture2D deathbackground = LoadTexture("assets/death.png");

    bool gDeath = false;
    bool pDeath = false;

    Map Levels[2];

    Vector2 screenCenter = {(GetScreenWidth() - 1024 * 0.5f) / 2, (GetScreenHeight() - 1024 * 0.5f) / 2};

    Button button("assets/button.png", screenCenter);
    Texture2D congrats = LoadTexture("assets/escapeText.png");
    Texture2D exitText = LoadTexture("assets/exitText.png");
    Texture2D deathText = LoadTexture("assets/deathText.png");

    Map Level1("assets/Map_Assets/Level1.txt", 4, 9, 34, "assets/Map_Assets/Dungeon_Bricks_Shadow.png");
    Map Level2("assets/Map_Assets/Level2.txt", 46, 24, 34, "assets/Map_Assets/Full.png");

    Sound buttonSound = LoadSound("assets/Music/Play_Button.wav"); // SOUND SYSTEM
    Sound coinSound = LoadSound("assets/Music/Coin.wav");
    Sound HealthSound = LoadSound("assets/Music/Health.wav");
    Sound enemyAttack = LoadSound("assets/Music/EnemyAttack.wav");
    Sound enemyDeath = LoadSound("assets/Music/EnemyDeath.wav");
    Sound playerAttackSound = LoadSound("assets/Music/PlayerAttack.wav");
    Sound gameOverSound = LoadSound("assets/Music/GameOver.wav");
    PlaySound(buttonSound);
    StopSound(buttonSound);
    PlaySound(coinSound);
    StopSound(coinSound);
    PlaySound(HealthSound);
    StopSound(HealthSound);
    SetSoundVolume(playerAttackSound, 3.0);
    SetSoundVolume(HealthSound, 3.0);
    Music bgm = LoadMusicStream("assets/Music/background.mp3");
    PlayMusicStream(bgm);
    Levels[0] = Level1;
    Levels[1] = Level2;

    int currLevel = 0;

    Player PlayerTest(Levels[currLevel].getSpawn());
    Golem golem(Levels[currLevel].getEnemySpawn(currEnemy));
    cout << "Main" << endl;
    currEnemy++;
    globalGolem = &golem;

    golem.SetTarget(&PlayerTest.Position);
    golem.SetMap(&Levels[currLevel]);

    TextManager Text;

    Camera2D camera = {0};
    camera.zoom = 3.0f;

    HighScoreManager high("HighScore.txt");
    bool newHighScore = false;

    ///////////////////////////////// INITIAL SET UP ENDS ///////////////////////////////////////////////////////

    while (!WindowShouldClose())
    {

        float dt = GetFrameTime(); /// FOR ANIMATIONS

        /////////////////////////////////// CAMERA LOGIC ///////////////////////////////////

        Vector2 playerCenter = {PlayerTest.Position.x + tileSize / 2, PlayerTest.Position.y + tileSize / 2};

        float halfWidth = (float)GetScreenWidth() / (2.0f * camera.zoom);
        float halfHeight = (float)GetScreenHeight() / (2.0f * camera.zoom);

        camera.target.x = Clamp(playerCenter.x, halfWidth, mapCols * tileSize - halfWidth);
        camera.target.y = Clamp(playerCenter.y, halfHeight, mapRows * tileSize - halfHeight);
        camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

        /////////////////////////////////// CAMERA LOGIC ///////////////////////////////////

        BeginDrawing();
        ClearBackground(BLACK);
        UpdateMusicStream(bgm);
        if (pDeath)
        {
            PauseMusicStream(bgm);
        }
        else
        {
            ResumeMusicStream(bgm);
        }

        if (!gameStarted)
        {

            DrawTexturePro(background, {0, 0, (float)(int)background.width, (float)(int)background.height}, {0, 0, (float)(int)GetScreenWidth(), (float)(int)GetScreenHeight()}, {0.0f, 0.0f}, 0.0f, WHITE);

            button.Draw();

            if (button.isClicked())
            {
                PlaySound(buttonSound);

                gameStarted=true;
            }
        }
        else if (gameOver)
        {

            if (high.Check(PlayerTest.getScore()))
            {
                Text.Create("New High Score!", {10, 10}, YELLOW, 60, 1000.0F);
                newHighScore = true;
            }

            DrawTexturePro(exitbackground, {0, 0, (float)(int)exitbackground.width, (float)(int)exitbackground.height}, {0, 0, (float)(int)GetScreenWidth(), (float)(int)GetScreenHeight()}, {0.0f, 0.0f}, 0.0f, WHITE);

            float scale = 0.5f;
            float scaledW = congrats.width * scale;
            float scaledH = congrats.height * scale;
            Vector2 center = {(GetScreenWidth() - scaledW) / 2.0f, (GetScreenHeight() - scaledH) / 2.0f};

            DrawTexturePro(congrats, {0, 0, (float)congrats.width, (float)congrats.height}, {center.x, center.y, scaledW, scaledH}, {0.0f, 0.0f}, 0.0f, WHITE);

            DrawTexturePro(exitText, {0, 0, (float)exitText.width, (float)exitText.height}, {center.x, (GetScreenHeight() + center.y) / 2, (float)(int)exitText.width * scale, (float)(int)exitText.height * scale}, {0.0f, 0.0f}, 0.0f, WHITE);

            Text.Draw();
            if (IsKeyPressed(KEY_ESCAPE))
            {
                break;
            }
        }
        else if (pDeath)
        {

            ClearBackground(BLACK);

            DrawTexturePro(deathbackground, {0, 0, (float)(int)deathbackground.width, (float)(int)deathbackground.height}, {0, 0, (float)(int)GetScreenWidth(), (float)(int)GetScreenHeight()}, {0.0f, 0.0f}, 0.0f, WHITE);

            float scale = 0.8f;
            float scaledW = deathText.width * scale;
            float scaledH = deathText.height * scale;
            Vector2 center = {(GetScreenWidth() - scaledW) / 2.0f, (GetScreenHeight() - scaledH) / 2.0f};

            DrawTexturePro(deathText, {0, 0, (float)deathText.width, (float)deathText.height}, {center.x, center.y, scaledW, scaledH}, {0.0f, 0.0f}, 0.0f, WHITE);
            if (!gOplayed) {
                PlaySound(gameOverSound);
                gOplayed = true;}

            if (IsKeyPressed(KEY_SPACE))
            {   gOplayed = false;

                Levels[0].Reset("assets/Map_Assets/Level1.txt", 4, 9, 34, "assets/Map_Assets/Dungeon_Bricks_Shadow.png");
                Levels[1].Reset("assets/Map_Assets/Level2.txt", 46, 24, 34, "assets/Map_Assets/Full.png");

                currLevel = 0;
                PlayerTest.ResetToSpawn(Levels[0].getSpawn());
                PlayerTest.Reset();
                pDeath = false;

                gDeath = false;
                golem.SetMap(&Levels[0]);
                golem.SetTarget(&PlayerTest.Position);
                golem.Reset({250, 250});
                gameStarted = true;
            }

            EndDrawing();
            continue;
        }
        else
        {
            BeginMode2D(camera);

            /////////////////////////////////// DRAWING ///////////////////////////////////
            Levels[currLevel].Draw();
            PlayerTest.Draw();
            golem.Draw();

            PlayerTest.HandleInput(&Levels[currLevel]);
            PlayerTest.Update(dt);
            golem.Update(dt);
            hSCooldown -= dt;
        cSCooldown -=dt;
        pACooldown -=dt;

        if (pACooldown <= 0.0f && IsKeyPressed(KEY_SPACE)){
                    PlayerTest.SetAnimation(PlayerTest.getAnim("attack"),8);
                    PlaySound(playerAttackSound);
                    pACooldown=0.2f;}
            /////////////////////////////////// GOLEM AND PLAYER INTERACTION /////////////////////////////////////////
            Rectangle playerHitbox = {PlayerTest.Position.x, PlayerTest.Position.y, 32, 32};
            Rectangle golemHitbox = {golem.Position.x, golem.Position.y, 32, 32};

            if (IsKeyPressed(KEY_SPACE) && !golem.IsDead())
            {
                if (CheckCollisionRecs(playerHitbox, golemHitbox))
                {
                    golem.TakeDamage(1);
                }
            }

            if (golem.IsDead() && !gDeath)
            {
                Text.Create("Golem DEAD", {PlayerTest.Position.x, PlayerTest.Position.y - 30}, GREEN, 10);
                gDeath = true;
            }

            if (golem.attackDone && !PlayerTest.IsDead())
            {
                if (CheckCollisionRecs(playerHitbox, golemHitbox))
                {
                    PlayerTest.TakeDamage(2);
                    golem.attackDone = false;

                    if (PlayerTest.IsDead())
                    {
                        PlayerTest.setActionStatus(true);
                        PlayerTest.SetAnimation(PlayerTest.getAnim("death"), 7);
                    }
                }
            }

            if (PlayerTest.IsDead())
            {
                if (!PlayerTest.getActionStatus())
                {
                    pDeath = true;
                }
            }

            /////////////////////////////////// GOLEM AND PLAYER INTERACTION /////////////////////////////////////////

            /////////////////////////////////// CHECK IF PLAYER AT EXIT /////////////////////////////////////////

            if (currLevel < 1 && Vector2Equals(PlayerTest.getGridPos(), Levels[currLevel].getExit()))
            {
                currLevel++;
                PlayerTest.ResetToSpawn(Levels[currLevel].getSpawn());
                golem.SetTarget(&PlayerTest.Position);
                golem.SetMap(&Levels[currLevel]);
            }
            /////////////////////////////////// GOLEM AND PLAYER INTERACTION /////////////////////////////////////////

            /////////////////////////////////// CHECK IF PLAYER AT EXIT /////////////////////////////////////////

            if (currLevel < 1 && Vector2Equals(PlayerTest.getGridPos(), Levels[currLevel].getExit()))
            {
                currLevel++;
                PlayerTest.ResetToSpawn(Levels[currLevel].getSpawn());
                golem.SetTarget(&PlayerTest.Position);
                golem.SetMap(&Levels[currLevel]);
            }
            /////////////////////////////////// CHECK IF PLAYER AT EXIT /////////////////////////////////////////

            /////////////////////////////////// COLLECTING ITEMS LOGIC /////////////////////////////////////////
            Vector2 tilePos = {PlayerTest.Position.x / 32, PlayerTest.Position.y / 32};

            if (Levels[currLevel].getMap()[(int)tilePos.y][(int)tilePos.x].collectable) ///// IS CURRENT TILE A COLLECTABLE?
            {
                Collectable *c = Levels[currLevel].getcollectables()[(int)tilePos.y][(int)tilePos.x];
                if (!c->IsCollected())
                {   if (c->getType() == "Health" && hSCooldown <= 0.0f) {
                    PlaySound(HealthSound);
                    hSCooldown = 0.2f;
                } else if (c->getType() == "Score" && cSCooldown <= 0.0f) {
                    PlaySound(coinSound);
                    cSCooldown = 0.2f;}
                    c->Collect();

                    /////////////////////////// HEALTH COLLECTABLE ////////////////////////////////

                    Vector2 textPos = {PlayerTest.Position.x, PlayerTest.Position.y - 10};

                    if (c->getType() == "Health")
                    {
                        PlayerTest.setHealth(c->getValue());
                        Text.Create("+10 Health", textPos, GREEN);
                    }

                    /////////////////////////// SCORE COLLECTABLE ////////////////////////////////s

                    else
                    {
                        PlayerTest.setScore(c->getValue());
                        Text.Create("+10 Score", textPos, GREEN);
                    }
                }
            }

            ////////////////////////// TEXT DISPLAYING AMOUNT OF SCORE/HEALTH GAINED ///////////////////////////
            Text.Update();
            Text.Draw();

            DrawText("Move: WASD | Attack: SPACE | Exit: ESC", 10, 10, 20, RED);

            if (currLevel == 1 && Vector2Equals(PlayerTest.getGridPos(), Levels[currLevel].getExit()))
            {
                gameOver = true;
            }

            EndMode2D();
        }

        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(exitbackground);
    UnloadTexture(congrats);
    UnloadTexture(exitText);
    UnloadTexture(deathbackground);
    UnloadTexture(deathText);
    StopMusicStream(bgm);
    UnloadMusicStream(bgm);
    UnloadSound(HealthSound);
    UnloadSound(buttonSound);
    UnloadSound(coinSound);
    UnloadSound(gameOverSound);
    UnloadSound(playerAttackSound);
    UnloadSound(enemyAttack);
    UnloadSound(enemyDeath);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
