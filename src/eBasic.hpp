#include "enemy.hpp"

#pragma once

namespace enemy
{
    class Basic : public Enemy
    {
    public:
        Basic(map::Tile *startTile, EnemyDir firstDir, Game &game);
        void Draw(Game &game) override;
        void Update(Game &game) override;
    };
}