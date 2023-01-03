#include <raylib.h>
#include "../../src/map.hpp"

namespace enemy
{
    enum class EnemyDir : int
    {
        RIGHT = 1,
        LEFT = -1,
        DOWN = 20,
        UP = -20,
    };

    enum class EnemySpeed : int
    {
        FAST = 500,
        MEDIUM = 400,
        SLOW = 300,
    };

    class Enemy
    {
    private:
        EnemyDir lastDir;
        EnemyDir dir;
        float x;
        float y;
        EnemySpeed speed;

        bool shouldDraw;
        bool isLeft;

        map::Tile* currentTile;
    public:
        int GetCurrentTileId();
        void CalcNewDir(map::Map& map);
        void UpdateAndDraw(map::Map& map);
        void move(EnemyDir& dir);

        Enemy(map::Tile* startTile, EnemySpeed inSpeed); //Delete this constructor for main game
        Enemy(map::Tile* startTile, EnemySpeed inSpeed, EnemyDir firstDir);
    };
}