#pragma once
#include <vector>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900
#define TILES_SIZE 80

namespace map
{
    enum class GroundType : int
    {
        GD_UNKNOWN = 0,
        GD_DIRT = 1,
        GD_SAND = 2,
        GD_ROAD = 3,
        GD_GRASS = 4,
        GD_EXIT = 5,
    };

    class Tile
    {
    private:
        int id;
        int x;
        int y;
        bool free;

    public:

        GroundType groundType;

        Tile(int inX = -1, int inY = -1, int inId = -1);
        int getId();
        int X();
        int Y();
        bool isFree();
        bool isRoad();
        int worldX();
        int worldY();
    };

    class Map
    {
    private:
        int entryId;
        int exitId;
        int secondaryExitId;

        void initMap(const char* level, int entry);

    public:
        std::vector<map::Tile> tilesArray;

        Map();
        Map(const char* level, int entry);
        ~Map();
        
        int getExit();
        int getSecondaryExit();

        int getEntry();

        void DrawTile() const;

        void changeMap(const char* newOne, int entry);

        void DrawFrame();
    };
}