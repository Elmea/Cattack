#include "map.hpp"
#include "entity.hpp"

#pragma once

class Game;

namespace enemy
{
    enum class EnemyDir : int
    {
        RIGHT = 1,
        LEFT = -1,
        DOWN = 20,
        UP = -20,
    };

    enum class EnemySpeed : int
    {
        FAST = 500,
        MEDIUM = 400,
        SLOW = 300,
    };

    class Enemy : public entity::Entity
    {
    private:
        EnemyDir mLastDir;
        EnemySpeed mSpeed;

        map::Tile *mCurrentTile;

        void CalcNewDir(Game &game);
        void Move(const EnemyDir &dir);

    protected:
        EnemyDir mDir;
        unsigned int mStepsTaken = 0;
        bool mShouldDraw;

        bool mPlayAttack = false;
        bool mPlayDeath = false;
        int mAnimLoop = 0;
        bool mAttackDone = false;

        int mDamage;
        int mMoneyYield;
        int mLife = 100;

        int mHealAmount;
        int mLifeMax;
        int mFrame = 0;
        float mLastFrameRefresh = 0;
        int mTypeId = 0;
        bool mIsDead = false;

        bool mIsTouched = false;
        float mTimeTouched = 0;

        int mTimeSlowed = 0;
        int mSlowPower = 1;

    public:
        bool Isleft;

        bool isAlive = true;

        int GetCurrentTileId() const;
        virtual void Update(Game &game);
        void CalcNewDir(map::Map &map);
        virtual void Draw(Game &game) = 0;

        Enemy(map::Tile *startTile, const EnemySpeed &inSpeed, EnemyDir firstDir, const int& money, Game &game);

        void GetHeal();

        unsigned int GetStep() const { return mStepsTaken; }
        int GetLife() const { return mLife; }
        void SetLife(const int newLife);
        int GetMoneyYield() const { return mMoneyYield; }
        int GetTypeId() const { return mTypeId; }
        bool IsItDead();
        void SetSlowTime(const int time);
        void GetTouched();
    };
}