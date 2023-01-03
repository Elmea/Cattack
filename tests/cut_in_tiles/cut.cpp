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
    if (id >= 180)
        free = false;
    else 
        free = true;
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
    for (int j = 0; j < (SCREEN_HEIGHT - 100)/TILES_SIZE; j++)
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
        DrawLine(game.tilesArray[i].worldX(), game.tilesArray[i].worldY(), game.tilesArray[i + 180].worldX(), game.tilesArray[i + 180].worldY(), BLACK);
    }

    for (int i = 0; i < 10; i++)
    {
        DrawLine(game.tilesArray[i*20].worldX(), game.tilesArray[i*20].worldY(), game.tilesArray[i*20].worldX() + SCREEN_WIDTH, game.tilesArray[i*20].worldY(), BLACK);
    }
}

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cut test");

    Game game;

    Color list1[5] = {PURPLE, YELLOW, PINK, BLUE, GREEN};
    Color list2[5] = {BLUE, GREEN, ORANGE, PURPLE, YELLOW};

    // Main game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();

        for (int i = 0; i < (int)game.tilesArray.size(); i++)
        {
            if (game.tilesArray[i].isFree())
            {
                if (i%3 == 0)
                {
                    DrawRectangle(game.tilesArray[i].worldX(), game.tilesArray[i].worldY(), TILES_SIZE, TILES_SIZE, list1[i%5]);
                    DrawText(TextFormat("%d : (%d , %d)", game.tilesArray[i].getId(), game.tilesArray[i].X(), game.tilesArray[i].Y()), game.tilesArray[i].worldX() + 10, game.tilesArray[i].worldY() + 10, 12, BLACK);
                }
                else 
                {
                    DrawRectangle(game.tilesArray[i].worldX(), game.tilesArray[i].worldY(), TILES_SIZE, TILES_SIZE, list2[i%5]);
                    DrawText(TextFormat("%d : (%d , %d)", game.tilesArray[i].getId(), game.tilesArray[i].X(), game.tilesArray[i].Y()), game.tilesArray[i].worldX() + 10, game.tilesArray[i].worldY() + 10, 12, BLACK);
                }
            }
        }

        DrawFraming(game);

        EndDrawing();
    }
    return 0;
}