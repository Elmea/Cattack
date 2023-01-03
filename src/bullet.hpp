#pragma once

#include <raylib.h>

#include "enemy.hpp"
#include "turret.hpp"


namespace bullet
{
    class Bullet : public entity::Entity
    {
    protected:
        turret::Turret* mParent;
        enemy::Enemy* mTarget;
        Vector2 mDirection;
        float mSpeed;
    public:
        Bullet(turret::Turret* t, enemy::Enemy* e, const float& s);
        ~Bullet();
        virtual void Update();
        turret::Turret* GetParent() const {return mParent;}
        enemy::Enemy* GetTarget() const {return mTarget;}
    };

    class ClassicBullet : public Bullet
    {
    public:
        ClassicBullet(turret::Turret* const &t, enemy::Enemy* const &e);
        ~ClassicBullet();
        void Draw(Texture2D& tileSheet) override;
    };

    class SlowingBullet : public Bullet
    {
    public:
        SlowingBullet(turret::Turret* const &t, enemy::Enemy* const &e);
        ~SlowingBullet();
        void Update() override;
        void Draw(Texture2D& tileSheet) override;
    };

    class ExplosiveBullet : public Bullet
    {
    private:
        float mBlastRadius = 80.f;
    public:
        ExplosiveBullet(turret::Turret* const &t, enemy::Enemy* const &e);
        ~ExplosiveBullet();
        float GetBlastRadius() const { return mBlastRadius; }
        void Draw(Texture2D& tileSheet) override;
    };
};