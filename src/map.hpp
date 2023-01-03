#pragma once
#include <vector>
#include <raylib.h>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900
#define TILES_SIZE 80
namespace enemy
{
    enum class EnemyDir : int;
}

namespace map
{
    enum class GroundType : int
    {
        GD_UNKNOWN,
        GD_DIRT,
        GD_SAND,
        GD_ROAD,
        GD_GRASS,
        GD_EXIT,
        GD_BUSH,
        GD_GR_ROCK,
        GD_SD_ROCK,
        GD_WAY_UP,
        GD_WAY_DOWN,
        GD_WAY_RIGHT,
        GD_WAY_LEFT,

    };

    class Tile
    {
    private:
        int id = -1;
        int x; // x on the frame
        int y; // y on the frame
        bool free = true; // false is there is already a tower on this tile

    public:
        char* label;

        GroundType groundType;

        Tile(const int& inX = -1, const int& inY = -1, const int& inId = -1);
        int GetId() const { return id; }
        int X() const { return x; }
        int Y() const { return y; }
        bool IsFree();
        void SetFree(bool availability);
        bool IsRoad();
        int WorldX();
        int WorldY();
    };

    class Map
    {
    private:
        int mEntryId; //Where enemies spwn
        int mExitId = -1; //The tile we have to defend
        int mSecondaryExitId = -1;
        enemy::EnemyDir mFirstDir;

        void InitMap(const char* level, int entry, enemy::EnemyDir INfirstDir);

    public:
        std::vector<map::Tile> tilesArray;

        Map();
        Map(const char* level, int entry, enemy::EnemyDir INfirstDir);
        ~Map();
        int GetExit() const { return mExitId; }
        int GetSecondaryExit() const { return mSecondaryExitId; }
        int GetEntry() const { return mEntryId; }

        void DrawTiles(Texture2D& tileSheet) const;

        void ChangeMap(const char* newOne, int entry, enemy::EnemyDir INfirstDir); //Init a new map from char*

        void DrawFrame();

        enemy::EnemyDir GetFirstDir() const { return mFirstDir; }

        Tile& GetTile(const int& x, const int& y);
    };
}