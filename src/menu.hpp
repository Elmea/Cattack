#pragma once

class Game;

namespace menu
{

    enum class MenuSelection
    {
        ME_MAIN,
        ME_LEADERBOARD,
        ME_CREDIT,
        ME_PAUSE,
        ME_GAMEOVER,
    };

    class Menu
    {
    public:
        MenuSelection menuSelected;

        void Display(Game& game);
        bool shouldDisplay;

        Menu();
    };
}