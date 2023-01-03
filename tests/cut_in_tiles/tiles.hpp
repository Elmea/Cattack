#pragma once
#include <vector>

class Tile
{
private:
    int id;
    int x;
    int y;
    bool free;
public:
    Tile(int inX, int inY, int inId);
    int getId();
    int X();
    int Y();
    bool isFree();
    int worldX();
    int worldY();
};

class Game
{
public:
    std::vector<Tile> tilesArray;

    Game();
    ~Game();
};