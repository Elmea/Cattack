#include <raylib.h>

#include "math/ourmath.hpp"
#include "eStrong.hpp"
#include "game.hpp"

using namespace enemy;

Strong::Strong(map::Tile* startTile, EnemyDir firstDir, Game& game) : Enemy(startTile, EnemySpeed::SLOW, firstDir, 20, game) 
{
    mDamage = 3;
    mLife *= 2;
    mLifeMax *= 2.5f;
    mTypeId = 3;
};

void Strong::Draw(Game& game)
{
    if (game.debug)
    {
        DrawText("Strong", x + 20, y - 10, 10, BLACK);
        DrawText(TextFormat("hp : %d", mLife), x + 20, y, 10, BLACK);
        DrawText(TextFormat("step : %d", GetStep()), x + 20, y + 10, 10, BLACK);
    }

    mLastFrameRefresh += GetFrameTime();

    if (!mPlayAttack && !mPlayDeath)
    {
        Vector2 healthBar = Lerp((Vector2){0, 5}, (Vector2){30, 5}, (float)mLife / (float)mLifeMax);

        DrawRectangleV((Vector2){x - 15, y - 30}, healthBar, GREEN);

        DrawRectangleLines(x - 15, y - 30, 30, 5, BLACK);

        if (mLastFrameRefresh >= 0.1f)
        {
            mFrame++;
            mLastFrameRefresh = 0;
        }

        Rectangle src = (Rectangle){0, 32*(float)mFrame, 32, 32};
        if (mDir == EnemyDir::LEFT)
            src = (Rectangle){32, 32*(float)mFrame, -32, 32};

        Rectangle dest = (Rectangle){x, y, 64, 64};

        if (mFrame > 6)         
            mFrame = 0;
        
        if (mTimeSlowed > 0)
            DrawTexturePro(game.ressources.chikBoy, src, dest, (Vector2){32, 32}, 0, BLUE);
        else if (mIsTouched)
            DrawTexturePro(game.ressources.chikBoy, src, dest, (Vector2){32, 32}, 0, RED);
        else
            DrawTexturePro(game.ressources.chikBoy, src, dest, (Vector2){32, 32}, 0, WHITE);
    }   
    else if (!mAttackDone && mPlayAttack)
    {
        if (mLastFrameRefresh >= 0.3f)
        {
            mFrame++;
            mLastFrameRefresh = 0;
        }

        Rectangle src = (Rectangle){0, 32*(float)mFrame, 32, 32};
        if (mDir == EnemyDir::LEFT)
            src = (Rectangle){32, 32*(float)mFrame, -32, 32};

        Rectangle dest = (Rectangle){x, y, 64, 64};

        if (mFrame > 4) 
        {
            mFrame = 0;
            mAnimLoop++;
        }
        if (mAnimLoop == 2)
            mAttackDone = true;
        DrawTexturePro(game.ressources.chikBoyIdle, src, dest, (Vector2){32, 32}, 0, WHITE);
    }   
    else if (!mIsDead&& mPlayDeath)
    {
        if (mLastFrameRefresh >= 0.3f)
        {
            mFrame++;
            mLastFrameRefresh = 0;
        }

        Rectangle src = (Rectangle){0, 32*(float)mFrame, 32, 32};
        if (mDir == EnemyDir::LEFT)
            src = (Rectangle){32, 32*(float)mFrame, -32, 32};

        Rectangle dest = (Rectangle){x, y, 64, 64};

        if (mFrame > 5) 
        {
            mFrame = 0;
            mAnimLoop++;
        }
        if (mAnimLoop)
        {
            mIsDead = true;
        }
        DrawTexturePro(game.ressources.chikBoyIdle, src, dest, (Vector2){24, 24}, 0, ColorAlpha(WHITE, 1.0f - (float)mFrame/5));
    }   
}

void Strong::Update(Game& game)
{
    this->Enemy::Update(game);
}