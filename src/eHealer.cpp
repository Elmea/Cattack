#include <raylib.h>

#include "math/ourmath.hpp"
#include "eHealer.hpp"
#include "game.hpp"

using namespace enemy;

Healer::Healer(map::Tile* startTile, EnemyDir firstDir, Game& game) : Enemy(startTile, EnemySpeed::FAST, firstDir, 10, game) 
{
    mLastHealTimer = GetTime();
    mTypeId = 2;
}

void Healer::ResetHealTimer()
{
    mLastHealTimer = GetTime();
}

bool Healer::CanHeal()
{
    mHealCooldown += GetFrameTime();
    return (mHealCooldown >= 4.0f) && (isAlive);
}

void Healer::HealNearEnemies(Game& game)
{ 
    PlaySound(game.ressources.heal);
    for (std::unique_ptr<enemy::Enemy>& enemy : game.enemies)
    {
        Vector2 myPos {this->x, this->y};
        Vector2 theirPos {enemy->x, enemy->y};

        if (Distance2D(myPos, theirPos) < mRange)
        {
            enemy->GetHeal();
            mStopTimer = 0;
            mIsHealing = true;
        }
    }

    this->GetHeal();
    mHealCooldown = 0;
}

void Healer::Draw(Game& game)
{
    // === [Animations gestion] ===

    mLastFrameRefresh += GetFrameTime();

    if (mIsHealing && !mPlayAttack && !mPlayDeath && !mIsDead) //Is Healing
    {
        if (mLastFrameRefresh >= 0.1f)
        {
            mFrame++;
            mLastFrameRefresh = 0;
        }

        Rectangle src = (Rectangle){0, 48*(float)mFrame, 48, 48};
        if (mDir == EnemyDir::LEFT)
            src = (Rectangle){48, 48*(float)mFrame, -48, 48};

        Rectangle dest = (Rectangle){x-10, y-20, 68, 68};

        if (mFrame > 5) 
        {
            mFrame = 0;
        }

        if (mDir == EnemyDir::RIGHT || mDir == EnemyDir::LEFT)
            DrawEllipse(x, y, mRange, TILES_SIZE/2, ColorAlpha(BLUE, 0.2f));
        else
            DrawEllipse(x, y, TILES_SIZE/2, mRange, ColorAlpha(BLUE, 0.2f));

        DrawTexturePro(game.ressources.witchHealing, src, dest, (Vector2){24, 24}, 0, WHITE);
    } 
    else if (!mPlayAttack && !mPlayDeath) //Is alive and walking
    {
        if (mLastFrameRefresh >= 0.1f)
        {
            mFrame++;
            mLastFrameRefresh = 0;
        }

        Rectangle src = (Rectangle){0, 41*(float)mFrame, 20, 40};
        if (mDir == EnemyDir::LEFT)
            src = (Rectangle){20, 41*(float)mFrame, -20, 40};

        Rectangle dest = (Rectangle){x+20, y+10, 30, 51};

        if (mFrame > 8)         
            mFrame = 0;
        
        if (mTimeSlowed > 0)
            DrawTexturePro(game.ressources.witchRun, src, dest, (Vector2){32, 32}, 0, BLUE);
        else if (mIsTouched)
            DrawTexturePro(game.ressources.witchRun, src, dest, (Vector2){32, 32}, 0, RED);
        else
            DrawTexturePro(game.ressources.witchRun, src, dest, (Vector2){32, 32}, 0, WHITE);
    }   
    else if (!mAttackDone && mPlayAttack) //Is attacking
    {
        if (mLastFrameRefresh >= 0.3f)
        {
            mFrame++;
            mLastFrameRefresh = 0;
        }

        Rectangle src = (Rectangle){0, 46*(float)mFrame, 51, 46};
        if (mDir == EnemyDir::LEFT)
            src = (Rectangle){51, 46*(float)mFrame, -51, 46};

        Rectangle dest = (Rectangle){x, y+10, 51, 46};

        if (mFrame > 9) 
        {
            mFrame = 0;
            mAnimLoop++;
        }
        if (mAnimLoop == 1)
            mAttackDone = true;
        DrawTexturePro(game.ressources.witchAttack, src, dest, (Vector2){32, 32}, 0, WHITE);
    }   
    else if (!mIsDead && mPlayDeath) //Is dying
    {
        if (mLastFrameRefresh >= 0.3f)
        {
            mFrame++;
            mLastFrameRefresh = 0;
        }

        Rectangle src = (Rectangle){0, 38*(float)mFrame, 27, 38};
        if (mDir == EnemyDir::LEFT)
            src = (Rectangle){27, 38*(float)mFrame, -27, 38};

        Rectangle dest = (Rectangle){x, y, 37, 48};

        if (mFrame > 11) 
        {
            mFrame = 0;
            mAnimLoop++;
        }
        if (mAnimLoop)
        {
            mIsDead = true;
        }
        DrawTexturePro(game.ressources.witchDeath, src, dest, (Vector2){24, 24}, 0, WHITE);
    }     

    if (isAlive && !mPlayAttack)
    {
        Vector2 healthBar = Lerp((Vector2){0, 5}, (Vector2){30, 5}, (float)mLife / (float)mLifeMax);

        DrawRectangleV((Vector2){x - 15, y - 30}, healthBar, GREEN);

        DrawRectangleLines(x - 15, y - 30, 30, 5, BLACK);
    }

    // === [Debug] ===

    if (game.debug)
    {
        DrawText("Healer", x + 20, y - 10, 10, BLACK);
        DrawText(TextFormat("hp : %d", mLife), x + 20, y, 10, BLACK);
        DrawText(TextFormat("step : %d", GetStep()), x + 20, y + 10, 10, BLACK);

        Color col = BLUE;
        col.a = 100;
        DrawCircle(x, y, mRange, col);
        DrawText(TextFormat("Cooldown : %2.0f", 4 - mHealCooldown), x - 80, y - 5, 10, BLACK);
    }
}

void Healer::Update(Game& game)
{
    if (CanHeal() && y <= 720 && y > 0 && x > 0 && x < SCREEN_WIDTH)
    {
        HealNearEnemies(game);
    }
    else if (!mIsHealing)
    {
        this->Enemy::Update(game);
    }
    else
    {
        mStopTimer += GetFrameTime();
        if (mStopTimer >= 0.5f)
        {
            mIsHealing = false;
        }
    }
}
