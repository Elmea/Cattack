#include "tiles.hpp"
#include <raylib.h>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900
#define TILES_SIZE 80

Tile::Tile(int inX, int inY, int inId)
{
    x = inX;
    y = inY;
    id = inId;
}

int Tile::getId()
{
    return id;
}

int Tile::X()
{
    return x;
}

int Tile::Y()
{
    return y;
}

int Tile::worldX()
{
    return x*TILES_SIZE;
}

int Tile::worldY()
{
    return y*TILES_SIZE;
}

bool Tile::isFree()
{
    return free;
}

Game::Game()
{
    int count = 0;
    for (int j = 0; j < (SCREEN_HEIGHT - 120)/TILES_SIZE; j++)
    {
        for (int i = 0; i < SCREEN_WIDTH/TILES_SIZE; i++)
        {
            tilesArray.push_back({i, j, count});
            count++;
        }
    }
}

Game::~Game()
{
    tilesArray.clear();
}

void DrawFraming(Game& game)
{
    for (int i = 0; i < 20; i++)
    {
        DrawLine(game.tilesArray[i].worldX(), game.tilesArray[i].worldY(), game.tilesArray[i + 160].worldX(), game.tilesArray[i + 160].worldY() + 80, BLACK);
    }

    for (int i = 0; i < 10; i++)
    {
        DrawLine(game.tilesArray[i*20].worldX(), game.tilesArray[i*20].worldY(), game.tilesArray[i*20].worldX() + SCREEN_WIDTH, game.tilesArray[i*20].worldY(), BLACK);
    }
}

GroundType Tile::getGroundType()
{
    return groundType;
}

void Tile::modifyGroundType(GroundType newType)
{
    groundType = newType;
}

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ground test");

    Game game;

    for (Tile& tile : game.tilesArray)
    {
        tile.modifyGroundType((GroundType)GetRandomValue(0, 4));
    }

    // Main game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();

        for (Tile& tile : game.tilesArray)
        {
            if (tile.isFree())
            {
                Color color;
                const char* label;
                switch (tile.getGroundType())
                {
                    case GD_UNKNOWN :
                        color = WHITE;
                        label = "UNKNOWN";
                        break;

                    case GD_DIRT :
                        color = BROWN;
                        label = "DIRT";
                        break;
                    
                    case GD_SAND :
                        color = YELLOW;
                        label = "SAND";
                        break;
                    
                    case GD_ROAD :
                        color = GRAY;
                        label = "ROAD";
                        break;
                    
                    case GD_GRASS :
                        color = GREEN;
                        label = "GRASS";
                        break;

                    default:
                        color = WHITE;
                        label = "UNKNOWN";
                        break;
                }
                DrawRectangle(tile.worldX(), tile.worldY(), TILES_SIZE, TILES_SIZE, color);
                DrawText(label, tile.worldX() + 10, tile.worldY() + 10, 12, BLACK);
                DrawText(TextFormat("%d", tile.getId()), tile.worldX() + 20, tile.worldY() + 30, 15, BLACK);
            }
        }

        DrawFraming(game);

        EndDrawing();
    }
    return 0;
}