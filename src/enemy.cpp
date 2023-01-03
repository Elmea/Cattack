#include <raylib.h>

#include "entity.hpp"
#include "game.hpp"
#include "math/ourmath.hpp"

#include "enemy.hpp"

using namespace enemy;

Enemy::Enemy(map::Tile* startTile, const EnemySpeed &inSpeed, EnemyDir firstDir, const int& money, Game& game) 
    : Entity(startTile->WorldX() + 40, startTile->WorldY() + 40)
    , mLifeMax(mLife + (game.GetWave() - 1)*25)
    , mMoneyYield(money)
{
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

    mDir = EnemyDir::RIGHT;
    mSpeed = inSpeed;
    mCurrentTile = startTile;
    mDir = firstDir;
    mShouldDraw = true;
    Isleft = false;
    mDamage = 1;
    mLife = mLifeMax;
}

int Enemy::GetCurrentTileId() const
{
    if (mCurrentTile == nullptr || mCurrentTile->GetId() > 179 || mCurrentTile->GetId() < 0)
        return -1;

    return mCurrentTile->GetId();
}

bool Enemy::IsItDead()
{
    return mIsDead;
}

void Enemy::GetTouched()
{
    mIsTouched = true;
    mTimeTouched = 0;
}

void Enemy::CalcNewDir(Game& game)
{
    mLastDir = mDir;
    int id = GetCurrentTileId();
    if (mCurrentTile->groundType == map::GroundType::GD_WAY_UP || mCurrentTile->groundType == map::GroundType::GD_WAY_DOWN || mCurrentTile->groundType == map::GroundType::GD_WAY_RIGHT || mCurrentTile->groundType == map::GroundType::GD_WAY_LEFT)
    {
        switch (mCurrentTile->groundType)
        {
        case map::GroundType::GD_WAY_UP:
            mDir = EnemyDir::UP;
            break;

        case map::GroundType::GD_WAY_DOWN:
            mDir = EnemyDir::DOWN;
            break;

        case map::GroundType::GD_WAY_RIGHT:
            mDir = EnemyDir::RIGHT;
            break;

        case map::GroundType::GD_WAY_LEFT:
            mDir = EnemyDir::LEFT;
            break;
        
        default:
            break;
        }
    }
    else if (!game.map.tilesArray[id + (int)mLastDir].IsRoad()) //try to go forward
    {
        while (!game.map.tilesArray[id + (int)mDir].IsRoad())
        {
            int rand = GetRandomValue(1,2); // 1 : Right            2 : left

            if (id == game.map.GetExit() || id == game.map.GetSecondaryExit())
            {
                Isleft = true;
                mFrame = 0;
                break;
            }

            if (!Isleft)
            {
                switch (mLastDir)
                {
                case EnemyDir::UP:
                    if (rand == 1)
                        mDir = EnemyDir::RIGHT;
                    else 
                        mDir = EnemyDir::LEFT;
                    break;

                case EnemyDir::DOWN:
                    if (rand == 1)
                        mDir = EnemyDir::LEFT;
                    else 
                        mDir = EnemyDir::RIGHT;
                    break;

                case EnemyDir::RIGHT:
                    if (rand == 1)
                        mDir = EnemyDir::DOWN;
                    else 
                        mDir = EnemyDir::UP;
                    break;

                case EnemyDir::LEFT:
                    if (rand == 1)
                        mDir = EnemyDir::UP;
                    else 
                        mDir = EnemyDir::DOWN;
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
    mCurrentTile = &game.map.tilesArray[id + (int)mDir];
}

void Enemy::Move(const EnemyDir& dir)
{
    if (mCurrentTile->groundType == map::GroundType::GD_DIRT)
    {
        switch (dir)
        {
        case EnemyDir::UP:
            y -= (float)mSpeed/400 / mSlowPower;
            break;

        case EnemyDir::DOWN:
            y += (float)mSpeed/400 / mSlowPower;
            break;

        case EnemyDir::RIGHT:
            x += (float)mSpeed/400 / mSlowPower;
            break;

        case EnemyDir::LEFT:
            x -= (float)mSpeed/400 / mSlowPower;
            break;

        default:
            break;
        }
        mStepsTaken += (int)mSpeed/400 / mSlowPower;
    }
    else
    {
        switch (dir)
        {
        case EnemyDir::UP:
            y -= (float)mSpeed/200 / mSlowPower;
            break;

        case EnemyDir::DOWN:
            y += (float)mSpeed/200 / mSlowPower;
            break;

        case EnemyDir::RIGHT:
            x += (float)mSpeed/200 / mSlowPower;
            break;

        case EnemyDir::LEFT:
            x -= (float)mSpeed/200 / mSlowPower;
            break;
        
        default:
            break;
        }
        mStepsTaken += (int)mSpeed/200 / mSlowPower;
    } 
}

void Enemy::Update(Game& game)
{
    if (mTimeSlowed > 0)
    {
        mTimeSlowed--;
        mSlowPower = 2;
    }
    else
    {
        mSlowPower = 1;
    }

    if (mLife <= 0 && isAlive)
    {
        isAlive = false;
        mPlayDeath = true;
        mFrame = 0;
        game.IncrementEnemiesKilled();
        mFrame = 0;
        game.money += mMoneyYield;
        PlaySound(game.ressources.death);
    }

    if(x < -200 || x > SCREEN_WIDTH + 200 || y < -200 || y > SCREEN_HEIGHT + 200)
    {
        isAlive = false;
        mIsDead = true;
        game.IncrementEnemiesKilled();
        game.money += mMoneyYield;
    }

    if (isAlive)
    {
        if ((GetCurrentTileId() != -1) && !(Isleft) && mLife != 0)
        {
            int tileId = GetCurrentTileId();
            float localX = x - game.map.tilesArray[tileId].WorldX(); //get coordinate on the tile
            float localY = y - game.map.tilesArray[tileId].WorldY();

            if ((localX <= 41 && localX >= 39) && (localY <= 41 && localY >= 39)) //Middle of the tile
            {
                CalcNewDir(game);
            }

            Move(mDir);
            mStepsTaken++;
        }
        else if (Isleft)
        {
            mPlayAttack = true;
        }
        else
        {
            mShouldDraw = false;
        }

        if (!mShouldDraw)
        {
            x = -100;
            y = -100;
        }
    }

    if (mAttackDone && mShouldDraw)
    {
        mShouldDraw = false;
        game.livesPlayer -= this->mDamage;
        isAlive = false;
        game.enemiesCounter--;  
    }
    mTimeTouched += GetFrameTime();
    if (mTimeTouched >= 0.15f)
    {
        mIsTouched = false;
    }
}

void enemy::Enemy::SetLife(int newLife) { mLife = newLife; }

void enemy::Enemy::SetSlowTime(int time) { mTimeSlowed = time; }

void Enemy::GetHeal()
{
    mHealAmount = mLifeMax / 5;
    mLife += mHealAmount;
    if(mLife > mLifeMax)
        mLife = mLifeMax;
}
