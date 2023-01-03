#include "enemy.hpp"

#pragma once

namespace enemy
{
    class Healer : public Enemy
    {
    private:
        double mHealCooldown = 0;
        double mLastHealTimer;
        bool mIsHealing = false;
        float mStopTimer = 0;
        int mRange = 80;

    public:
        Healer(map::Tile *startTile, EnemyDir firstDir, Game &game);
        void ResetHealTimer();
        bool CanHeal();
        void HealNearEnemies(Game &game);
        void Draw(Game &game) override;
        void Update(Game &game) override;
    };
}