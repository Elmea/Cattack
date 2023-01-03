#include "map.hpp"
#include <raylib.h>

using namespace map;

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

bool Tile::isRoad()
{
    if (groundType == GroundType::GD_ROAD || groundType == GroundType::GD_DIRT || groundType == GroundType::GD_EXIT)
    {
        return true;
    }

    return false;
}

Map::Map()
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

void Map::initMap(const char* level, int entry)
{
    int count = 0;
    int exitCount = 0;
    entryId = entry;
    for (int j = 0; j < (SCREEN_HEIGHT - 120)/TILES_SIZE; j++)
    {
        for (int i = 0; i < SCREEN_WIDTH/TILES_SIZE; i++)
        {
            tilesArray.push_back({i, j, count});

            if (level[count] == 'E')
            {
                tilesArray[count].groundType = map::GroundType::GD_EXIT;
                if (exitCount == 0)
                {
                    exitId = count;
                    exitCount++;
                }
                else if (exitCount == 1)
                {
                    secondaryExitId = count;
                    exitCount++;
                }
            }

            else if (level[count] == 'R')
            {
                tilesArray[count].groundType = map::GroundType::GD_ROAD;
            }
            else if (level[count] == 'D')
            {
                tilesArray[count].groundType = map::GroundType::GD_DIRT;
            }
            else if (level[count] == '_')
            {
                tilesArray[count].groundType = map::GroundType::GD_SAND;
            }
            else
            {
                tilesArray[count].groundType = map::GroundType::GD_GRASS;
            }

            count++;
        }
    }
}

Map::Map(const char* level, int entry)
{
    initMap(level, entry);
}

Map::~Map()
{
    tilesArray.clear();
}

void Map::DrawTile() const
{
    for (map::Tile tile : tilesArray)
    {
        if (tile.isFree())
        {
            Color color;
            const char* label;
            switch (tile.groundType)
            {
                case map::GroundType::GD_UNKNOWN :
                    color = WHITE;
                    label = "UNKNOWN";
                    break;

                case map::GroundType::GD_DIRT :
                    color = BROWN;
                    label = "DIRT";
                    break;
                
                case map::GroundType::GD_SAND :
                    color = YELLOW;
                    label = "SAND";
                    break;
                
                case map::GroundType::GD_ROAD :
                    color = GRAY;
                    label = "ROAD";
                    break;
                
                case map::GroundType::GD_GRASS :
                    color = GREEN;
                    label = "GRASS";
                    break;

                case map::GroundType::GD_EXIT :
                    color = {90, 204, 230, 255};
                    label = "EXIT";
                    break;

                default:
                    color = WHITE;
                    label = "UNKNOWN";
                    break;
            }
            DrawRectangle(tile.worldX(), tile.worldY(), TILES_SIZE, TILES_SIZE, color);
            if (0) //Replace by if (game.debug) in main code
            {
                DrawText(label, tile.worldX() + 10, tile.worldY() + 10, 12, BLACK);
                DrawText(TextFormat("%d", tile.getId()), tile.worldX() + 20, tile.worldY() + 30, 15, BLACK);
            }
        }
    }
}

void Map::DrawFrame()
{
    for (int i = 0; i < 20; i++)
    {
        DrawLine(tilesArray[i].worldX(), tilesArray[i].worldY(), tilesArray[i + 160].worldX(), tilesArray[i + 160].worldY() + 80, BLACK);
    }

    for (int i = 0; i < 10; i++)
    {
        DrawLine(tilesArray[i*20].worldX(), tilesArray[i*20].worldY(), tilesArray[i*20].worldX() + SCREEN_WIDTH, tilesArray[i*20].worldY(), BLACK);
    }
}

int Map::getExit()
{
    return exitId;
}

int Map::getSecondaryExit()
{
    return secondaryExitId;
}

int Map::getEntry()
{
    return entryId;
}

void Map::changeMap(const char* newOne, int entry)
{
    tilesArray.clear();

    initMap(newOne, entry);
}
