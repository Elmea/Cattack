#pragma once
#include <raylib.h>

#include "entity.hpp"
#include "enemy.hpp"

#define TURRET_SIZE 20

namespace turret
{
    class Turret : public entity::Entity
    {
    protected:
        float mRange;
        int mCost;
        int mDelayBetweenShell;
        int mDamage;
        float mRotation = 0;
        int mLevel = 1;
        virtual void Upgrade(int& funds) = 0;
    public:
        Turret(const float& vX, const float& vY, const float& r, const int& del, const int& dmg, const int& c);
        ~Turret();
        float GetRange() const { return mRange; }
        int GetAttDelay() const { return mDelayBetweenShell; }
        int GetCost() const { return mCost; }
        int GetDamage() const { return mDamage; }
        int GetLevel() const { return mLevel; }
        float& Rotation();
        virtual void Selected(int& funds, Sound& upgradeSound);
        void Rotate(enemy::Enemy* target);
    };

    class ClassicTurret : public Turret
    {
    protected:
        void Upgrade(int& funds) override;
    public:
        ClassicTurret(const float& vX, const float& vY);
        ~ClassicTurret();
        void Draw(Texture2D& tileSheet) override;
        void Selected(int& funds, Sound& upgradeSound) override;
    };

    class GlueTurret : public Turret
    {
    protected:
        void Upgrade(int& funds) override;
    public:
        GlueTurret(const float& vX, const float& vY);
        ~GlueTurret();
        void Draw(Texture2D& tileSheet) override;
        void Selected(int& funds, Sound& upgradeSound) override;
    };

    class RocketTurret : public Turret
    {
    protected:
        void Upgrade(int& funds) override;
    public:
        RocketTurret(const float& vX, const float& vY);
        ~RocketTurret();
        void Draw(Texture2D& tileSheet) override;
        void Selected(int& funds, Sound& upgradeSound) override;
    };
};