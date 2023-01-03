#include "enemyMove.hpp"

using namespace enemy;

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "move test");

    map::Map map;

    int RoadId[37] = {40, 41, 42, 62, 82, 83, 84, 85, 86, 106, 126, 125, 124, 104, 64, 44, 45, 46, 47, 48, 49, 50, 70, 90, 110, 130, 131, 132, 133, 134, 135, 115, 95, 96, 97, 98, 99};

    for (map::Tile& tile : map.tilesArray)
    {
        for (int roadId : RoadId)
        {
            if (tile.getId() == roadId)
            {
                tile.groundType = map::GroundType::GD_ROAD;
                break;
            }
            else 
            {
                tile.groundType = map::GroundType::GD_GRASS;
            }
        }

    }

    Enemy enemy1 {&map.tilesArray[40], EnemySpeed::MEDIUM};
    Enemy enemy2 {&map.tilesArray[40], EnemySpeed::FAST};
    Enemy enemy3 {&map.tilesArray[40], EnemySpeed::SLOW};

    // Main map loop
    while (!WindowShouldClose())
    {
        BeginDrawing();

        map.DrawTiles();

        map.DrawFrame();

        enemy1.UpdateAndDraw(map);

        enemy2.UpdateAndDraw(map);

        enemy3.UpdateAndDraw(map);

        EndDrawing();
    }
    return 0;
}