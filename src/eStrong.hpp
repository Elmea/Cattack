#include "enemy.hpp"

#pragma once

namespace enemy
{
    class Strong : public Enemy
    {
    public:
        Strong(map::Tile *startTile, EnemyDir firstDir, Game &game);
        virtual void Update(Game &game) override;
        void Draw(Game &game) override;
    };
}