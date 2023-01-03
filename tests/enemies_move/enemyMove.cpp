#include "enemyMove.hpp"

using namespace enemy;

int Enemy::GetCurrentTileId()
{
    return currentTile->getId();
}

void Enemy::CalcNewDir(map::Map& map)
{
    lastDir = dir;
    int id = GetCurrentTileId();
    if (!map.tilesArray[id + (int)lastDir].isRoad()) //try to go forward
    {
        while (!map.tilesArray[id + (int)dir].isRoad())
        {
            int rand = GetRandomValue(1,2); // 1 : Right            2 : left

            bool flag = false;
            for (int i = 0; i < 9; i++)
            {
                if (id == 19 + i*20)
                {
                    flag = true;
                    dir = EnemyDir::RIGHT;
                    break;
                }
            }
            if (flag)
                break;

            switch (lastDir)
            {
            case EnemyDir::UP:
                if (rand == 1)
                    dir = EnemyDir::RIGHT;
                else 
                    dir = EnemyDir::LEFT;
                break;

            case EnemyDir::DOWN:
                if (rand == 1)
                    dir = EnemyDir::LEFT;
                else 
                    dir = EnemyDir::RIGHT;
                break;

            case EnemyDir::RIGHT:
                if (rand == 1)
                    dir = EnemyDir::DOWN;
                else 
                    dir = EnemyDir::UP;
                break;

            case EnemyDir::LEFT:
                if (rand == 1)
                    dir = EnemyDir::UP;
                else 
                    dir = EnemyDir::DOWN;
                break;
            
            default:
                break;
            }
        }
    }
    currentTile = &map.tilesArray[id + (int)dir];
}

void Enemy::move(EnemyDir& dir)
{
    if (currentTile->groundType == map::GroundType::GD_DIRT)
    {
        switch (dir)
        {
        case EnemyDir::UP:
            y -= (int)speed/2;
            break;

        case EnemyDir::DOWN:
            y += (int)speed/2;
            break;

        case EnemyDir::RIGHT:
            x += (int)speed/2;
            break;

        case EnemyDir::LEFT:
            x -= (int)speed/2;
            break;

        default:
            break;
        }
    }
    else
    {
        switch (dir)
        {
        case EnemyDir::UP:
            y -= (int)speed;
            break;

        case EnemyDir::DOWN:
            y += (int)speed;
            break;

        case EnemyDir::RIGHT:
            x += (int)speed;
            break;

        case EnemyDir::LEFT:
            x -= (int)speed;
            break;
        
        default:
            break;
        }
    } 
}

void Enemy::UpdateAndDraw(map::Map& map)
{
    if (currentTile != nullptr)
    {
        int id = GetCurrentTileId();
        float localX = x - map.tilesArray[id].worldX(); //get coordinate on the tile
        float localY = y - map.tilesArray[id].worldY();

        if (localX == 40 && localY == 40) //Middle of the tile
        {
            CalcNewDir(map);
        }

        move(dir);

        DrawCircle(x, y, 5, RED);
    }
}

Enemy::Enemy(map::Tile* startTile, EnemySpeed inSpeed)
{
    x = 0;
    y = startTile->worldY() + 40;
    dir = EnemyDir::RIGHT;
    speed = inSpeed;
    currentTile = startTile;
    isLeft = false;
}
