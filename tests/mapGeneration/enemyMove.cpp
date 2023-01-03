#include "enemyMove.hpp"

using namespace enemy;

int Enemy::GetCurrentTileId()
{
    if (currentTile == nullptr || currentTile->getId() > 179 || currentTile->getId() < 0)
        return -1;

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

            if (id == map.getExit() || id == map.getSecondaryExit())
            {
                isLeft = true;
                break;
            }

            if (!isLeft)
            {
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
            else 
            {
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
            y -= (float)speed/400;
            break;

        case EnemyDir::DOWN:
            y += (float)speed/400;
            break;

        case EnemyDir::RIGHT:
            x += (float)speed/400;
            break;

        case EnemyDir::LEFT:
            x -= (float)speed/400;
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
            y -= (float)speed/200;
            break;

        case EnemyDir::DOWN:
            y += (float)speed/200;
            break;

        case EnemyDir::RIGHT:
            x += (float)speed/200;
            break;

        case EnemyDir::LEFT:
            x -= (float)speed/200;
            break;
        
        default:
            break;
        }
    } 
}

void Enemy::UpdateAndDraw(map::Map& map)
{
    if ((GetCurrentTileId() != -1) && !(isLeft))
    {
        int tileId = GetCurrentTileId();
        float localX = x - map.tilesArray[tileId].worldX(); //get coordinate on the tile
        float localY = y - map.tilesArray[tileId].worldY();

        if ((localX <= 41 && localX >= 39) && (localY <= 41 && localY >= 39)) //Middle of the tile
        {
            CalcNewDir(map);
        }

        move(dir);

        DrawCircle(x, y, 20, RED);
    }
    else
    {
        shouldDraw = false;
    }

    if (!shouldDraw)
    {
        x = -100;
        y = -100;
    }
}

Enemy::Enemy(map::Tile* startTile, EnemySpeed inSpeed, EnemyDir firstDir)
{
    x = startTile->worldX() + 40;

    y = startTile->worldY() + 40;

    switch (firstDir)
    {
    case enemy::EnemyDir::DOWN:
        y -= 80;
        break;
    
    case enemy::EnemyDir::UP:
        y += 80;
        break;

    case enemy::EnemyDir::RIGHT:
        x -= 80;
        break;

    case enemy::EnemyDir::LEFT:
        x += 80;
        break;

    default:
        break;
    }

    dir = EnemyDir::RIGHT;
    speed = inSpeed;
    currentTile = startTile;
    dir = firstDir;
    shouldDraw = true;
    isLeft = false;
}