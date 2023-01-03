#include <raylib.h>


#include "ui.hpp"
#include "map.hpp"
#include "game.hpp"

#include "menu.hpp"

using namespace menu;

bool keyLooker = false;

void Menu::Display(Game& game)
{
    if (shouldDisplay)
    {
        switch (menuSelected)
        {
        case MenuSelection::ME_MAIN:
            DrawTexturePro(game.ressources.textureMainMenu, (Rectangle){0, 0, 1100, 750}, (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, (Vector2){0, 0}, 0, WHITE);
            DrawText("Cattack", SCREEN_WIDTH/2 - 175, 50, 100, DARKPURPLE);
            if (ui::Button(SCREEN_WIDTH/2 - 60, 200, 120, 55, 50,  "Play", BLACK))
            {
                shouldDisplay = false;
                game.gameState = GameState::GM_MapSelection;
            }

            if (ui::Button(SCREEN_WIDTH/2 - 150, 400, 300, 50, 45, "Leaderboard", BLACK))
            {
                menuSelected = MenuSelection::ME_LEADERBOARD;
            }

            if (ui::Button(SCREEN_WIDTH/2 - 68, 600, 160, 50, 50, "Credit", BLACK))
            {
                menuSelected = MenuSelection::ME_CREDIT;
            }

            break;
        
        case MenuSelection::ME_LEADERBOARD:
            game.DrawLeaderboard();

            if (ui::Button(SCREEN_WIDTH - 60, 40, 'X', RED))
            {
                menuSelected = MenuSelection::ME_MAIN;
            }
            break;

        case MenuSelection::ME_CREDIT:

            DrawTexturePro(game.ressources.textureCredit, (Rectangle){0, 0, 1100, 750}, (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, (Vector2){0, 0}, 0, WHITE);
            DrawText("Team Developpement: ", SCREEN_WIDTH/4, SCREEN_HEIGHT/2.5, 70, WHITE);
            DrawText("- MASSETEAU Nino ", SCREEN_WIDTH/4, SCREEN_HEIGHT/2, 60, WHITE);
            DrawText("- GOUPIL Kristian ", SCREEN_WIDTH/4, SCREEN_HEIGHT/1.7, 60, WHITE);
            DrawText("- NERRAND Eliot ", SCREEN_WIDTH/4, SCREEN_HEIGHT/1.5, 60, WHITE);
            DrawText("Thank You For Playing Our Game", SCREEN_WIDTH/10, SCREEN_HEIGHT/5, 75, WHITE);

            DrawText("Artist: 9E0 (from itch.io)", SCREEN_WIDTH/4, SCREEN_HEIGHT/1.3, 60, WHITE);

            if(ui::Button(1375, 880, 50, 20, 1, "  ", BLANK))
            {
                PlaySound(game.ressources.musicMode);

                UnloadMusicStream(game.ressources.musics[0]);
                game.ressources.musics[0] = LoadMusicStream("assets/sounds/wiiSport.ogg");
                SetMusicVolume(game.ressources.musics[0], 0.60f);

                UnloadMusicStream(game.ressources.musics[1]);
                game.ressources.musics[1] = LoadMusicStream("assets/sounds/rick-astley.ogg");
                SetMusicVolume(game.ressources.musics[1], 0.60f);
            }

            if (ui::Button(SCREEN_WIDTH - 60, 40, 'X', RED))
            {
                menuSelected = MenuSelection::ME_MAIN;
            }
            break;

        case MenuSelection::ME_PAUSE:
            Color cache;
            cache = GRAY;
            cache.a = 100;
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, cache);
            DrawText("GAME PAUSED", SCREEN_WIDTH/4, SCREEN_HEIGHT/3, 100, WHITE);

            if ((ui::Button(SCREEN_WIDTH/2 - 100, 750, 200, 100, 45, "Resume", GRAY) || IsKeyPressed(KEY_P)) && keyLooker)
            {
                keyLooker = false;

                shouldDisplay = false;
                game.gameState = GameState::GM_ISRUNNING;
            }
            else
            {
                keyLooker = true;
            }
            break;

        case MenuSelection::ME_GAMEOVER:
            DrawTexturePro(game.ressources.textureMainMenu, (Rectangle){0, 0, 1100, 750}, (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, (Vector2){0, 0}, 0, WHITE);
            if (game.UpdateAndDrawGameOver())
            {
                game.Reset();
                menuSelected = MenuSelection::ME_MAIN;
            }
            break;

        default:
            break;
        }
    }
}

Menu::Menu()
{
    shouldDisplay = true;
    menuSelected = MenuSelection::ME_MAIN;
}