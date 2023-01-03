#include <raylib.h>

#include "math/ourmath.hpp"
#include "eBasic.hpp"
#include "game.hpp"

using namespace enemy;

Basic::Basic(map::Tile* startTile, EnemyDir firstDir, Game& game) : Enemy(startTile, EnemySpeed::MEDIUM, firstDir, 5, game) 
{
    mDamage = 2;
    mTypeId = 1;
};

void Basic::Draw(Game& game)
{
    if (game.debug)
    {
        DrawText("Basic", x + 20, y - 10, 10, BLACK);
        DrawText(TextFormat("hp : %d", mLife), x + 20, y, 10, BLACK);
        DrawText(TextFormat("step : %d", GetStep()), x + 20, y + 10, 10, BLACK);
    }

    mLastFrameRefresh += GetFrameTime();

    // === [Animations gestion] ===

    if (!mPlayAttack && !mPlayDeath) //Enemy is alive
    {
        Vector2 healthBar = Lerp((Vector2){0, 5}, (Vector2){30, 5}, (float)mLife / (float)mLifeMax);

        DrawRectangleV((Vector2){x - 15, y - 30}, healthBar, GREEN);

        DrawRectangleLines(x - 15, y - 30, 30, 5, BLACK);

        if (mLastFrameRefresh >= 0.1f)
        {
            mFrame++;
            mLastFrameRefresh = 0;
        }

        Rectangle src = (Rectangle){0, 16*(float)mFrame, 12, 16};
        if (mDir == EnemyDir::LEFT)
            src = (Rectangle){12, 16*(float)mFrame, -12, 16};

        Rectangle dest = (Rectangle){x, y, 36, 48};

        if (mFrame > 8)   
            mFrame = 0;

        if (mIsTouched)
            DrawTexturePro(game.ressources.meowKnight, src, dest, (Vector2){16, 16}, 0, RED);
        else if (mTimeSlowed > 0)
            DrawTexturePro(game.ressources.meowKnight, src, dest, (Vector2){16, 16}, 0, SKYBLUE);
        else
            DrawTexturePro(game.ressources.meowKnight, src, dest, (Vector2){16, 16}, 0, WHITE);
    }   
    else if (!mAttackDone && mPlayAttack) // Is attacking
    {
        if (mLastFrameRefresh >= 0.3f)
        {
            mFrame++;
            mLastFrameRefresh = 0;
        }

        Rectangle src = (Rectangle){0, 24*(float)mFrame, 40, 24};
        if (mDir == EnemyDir::LEFT)
            src = (Rectangle){40, 24*(float)mFrame, -40, 24};

        Rectangle dest = (Rectangle){x, y, 80, 48};

        if (mFrame > 4) 
        {
            mFrame = 0;
            mAnimLoop++;
            PlaySound(game.ressources.swordAttack);
        }
        if (mAnimLoop == 2)
            mAttackDone = true;
        
        DrawTexturePro(game.ressources.meowAttack, src, dest, (Vector2){24, 24}, 0, WHITE);
    }   
    else if (!mIsDead&& mPlayDeath) // Is dying
    {
        if (mLastFrameRefresh >= 0.4f)
        {
            mFrame++;
            mLastFrameRefresh = 0;
        }

        Rectangle src = (Rectangle){0, 15*(float)mFrame, 23, 15};
        if (mDir == EnemyDir::LEFT)
            src = (Rectangle){23, 15*(float)mFrame, -23, 15};

        Rectangle dest = (Rectangle){x, y, 60, 48};

        if (mFrame > 4) 
        {
            mFrame = 4;
            mIsDead = true;
        }
        DrawTexturePro(game.ressources.meowDeath, src, dest, (Vector2){24, 24}, 0, WHITE);
    }   
}

void Basic::Update(Game& game)
{
    this->Enemy::Update(game);
}