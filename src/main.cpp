#include <raylib.h>
#include <time.h>
#include <iostream>

#include "entity.hpp"
#include "game.hpp"
#include "menu.hpp"
#include "level.hpp"
#include "ui.hpp"

int main()
{
    const int screenWidth = SCREEN_WIDTH;
    const int screenHeight = SCREEN_HEIGHT;

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Cattack");
    InitAudioDevice();
    SetTargetFPS(60);

    {
    Game game;

    SetRandomSeed(time(NULL));

    game.PlayIntro();

    // Main game loop
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F3))
        {
            if (!game.debug)
                game.debug = true;
            else
                game.debug = false;
        };
        BeginDrawing();

        ClearBackground(WHITE);

        game.MainLoop();

        game.ressources.DrawAndUpdateVolume();

        EndDrawing();
    }

    game.ressources.UnloadSounds();
    game.ressources.UnloadMusics();
    game.ressources.UnloadTextures();
    }
    
    CloseAudioDevice();
    CloseWindow();

    return 0;
}