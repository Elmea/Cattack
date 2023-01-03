#include <vector>

typedef enum GroundType
{
    GD_UNKNOWN = 0,
    GD_DIRT = 1,
    GD_SAND = 2,
    GD_ROAD = 3,
    GD_GRASS = 4,
}GroundType;

class Tile
{
private:
    int id;
    int x;
    int y;
    bool free;
    GroundType groundType;

public:

    Tile(int inX, int inY, int inId);
    int getId();
    int X();
    int Y();
    bool isFree();
    int worldX();
    int worldY();
    GroundType getGroundType();
    void modifyGroundType(GroundType newType);
};

class Game
{
public:
    std::vector<Tile> tilesArray;

    Game();
    ~Game();
};