#include <raylib.h>
#include "map.hpp"
#include "enemyMove.hpp"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900
#define TILES_SIZE 80

const char* levels(int& entryId, enemy::EnemyDir& firstDir, int level)
{
    if (level == 1)
    {
        entryId = 40;

        firstDir = enemy::EnemyDir::RIGHT;

        return 
        "--------------------"
        "--------------------"
        "RRR-RRRRRRRR--------"
        "--R-R------R--RRR---"
        "--RRRRRRR--R--R-R---"
        "----R---R--R--R-R---"
        "----RRRRR--RRRR-RRRE"
        "--------------------"
        "--------------------";
    }

    if (level == 2)
    {
    entryId = 6;

    firstDir = enemy::EnemyDir::DOWN;

    return 
    "------R-----________"
    "--RRRRR----_____RRRE"
    "--R-------______R___"
    "--RRRRRRR_______R___"
    "--------R_______R___"
    "-----_RRRRRRRRRRR_RE"
    "--____R_R_______R_R_"
    "______RRR_______RRR_"
    "____________________";
    }

    if (level == 3)
    {
    entryId = 162;

    firstDir = enemy::EnemyDir::UP;

    return 
    "--------------E-----"
    "--------RDDDDDD-----"
    "--------R-----------"
    "--------RRDDDRR-----"
    "--------------R-----"
    "--RRRRRRRRRD--D-----"
    "--R--------DDDD-----"
    "--R-----------------"
    "--R-----------------";
    }

    if (level == 4)
    {
    entryId = 59;

    firstDir = enemy::EnemyDir::LEFT;

    return 
    "--------------------"
    "--------------------"
    "--------------DDRRRR"
    "--------------D-----"
    "ERRRRRRRRRRRRDD-----"
    "--------------------"
    "--------------------"
    "--------------------"
    "--------------------";
    }

    if (level == 5)
    {
    entryId = 6;

    firstDir = enemy::EnemyDir::DOWN;

    return 
    "______R_____________"
    "______R_____________"
    "______R___E_________"
    "______R___R_________"
    "______R___R_________"
    "______RRRRR_________"
    "__________R_________"
    "__________E_________"
    "____________________";
    }

    return "\0";
}

int main()
{
    int levelSelected = 1;

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "map generation test");

    int entryId;
    enemy::EnemyDir firstDir;
    SetRandomSeed(24452131);

    map::Map map{levels(entryId, firstDir, levelSelected), entryId};

    std::vector<enemy::Enemy> enemies;

    enemies.push_back({&map.tilesArray[map.getEntry()], enemy::EnemySpeed::MEDIUM, firstDir});

    enemies.push_back({&map.tilesArray[map.getEntry()], enemy::EnemySpeed::FAST, firstDir});

    enemies.push_back({&map.tilesArray[map.getEntry()], enemy::EnemySpeed::SLOW, firstDir});

    bool go = false;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(DARKGRAY);

        map.DrawTile();

        DrawText(TextFormat("%d/5 map", levelSelected), SCREEN_WIDTH/2 - 20, 720, 20, WHITE);
        DrawText("press space to run", SCREEN_WIDTH/2 - 50, 740, 20, WHITE);
        
        if (IsKeyPressed(KEY_SPACE))
        {
            go = true;
        }

        if (go)
        {
            for (enemy::Enemy& enemy : enemies)
            {
                enemy.UpdateAndDraw(map);
            }
        }

        if (IsKeyPressed(KEY_RIGHT))
        {
            if (levelSelected < 5)
            {           
                enemies.clear();
                levelSelected++;
                const char* newLevel = levels(entryId, firstDir, levelSelected);
                map.changeMap(newLevel, entryId);

                enemies.push_back({&map.tilesArray[map.getEntry()], enemy::EnemySpeed::MEDIUM, firstDir});

                enemies.push_back({&map.tilesArray[map.getEntry()], enemy::EnemySpeed::FAST, firstDir});

                enemies.push_back({&map.tilesArray[map.getEntry()], enemy::EnemySpeed::SLOW, firstDir});

                go = false;
            }
        }


        if (IsKeyPressed(KEY_LEFT))
        {
            if (levelSelected > 1)
            {    
            enemies.clear();
            levelSelected--;
            const char* newLevel = levels(entryId, firstDir, levelSelected);
            map.changeMap(newLevel, entryId);

            enemies.push_back({&map.tilesArray[map.getEntry()], enemy::EnemySpeed::MEDIUM, firstDir});

            enemies.push_back({&map.tilesArray[map.getEntry()], enemy::EnemySpeed::FAST, firstDir});

            enemies.push_back({&map.tilesArray[map.getEntry()], enemy::EnemySpeed::SLOW, firstDir});

            go = false;
            }
        }

        EndDrawing();
    }
    enemies.clear();

    return 0;
}