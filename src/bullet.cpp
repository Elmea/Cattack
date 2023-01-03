#include <math.h>

#include "bullet.hpp"


bullet::Bullet::Bullet(turret::Turret* t, enemy::Enemy* e, const float& s) : Entity(t->x + cosf(t->Rotation() * PI / 180.f) * 32.f, t->y+ sinf(t->Rotation() * PI / 180.f) * 32.f), mParent(t), mTarget(e), mSpeed(s)
{
    Vector2 dir = (Vector2){(float)e->x - (float)t->x, (float)e->y - (float)t->y};
    float magnitude = sqrtf(powf(dir.x, 2) + powf(dir.y, 2));
    mDirection = {dir.x / magnitude, dir.y / magnitude};
}

bullet::Bullet::~Bullet() {} 


void bullet::Bullet::Update()
{
    x += mDirection.x * mSpeed;
    y += mDirection.y * mSpeed; 
}

bullet::ClassicBullet::ClassicBullet(turret::Turret* const &t, enemy::Enemy* const &e) : bullet::Bullet(t, e, 10.f) {}

bullet::ClassicBullet::~ClassicBullet() {} 

void bullet::ClassicBullet::Draw(Texture2D& tileSheet)
{
    DrawTexturePro(tileSheet, {1216, 704, 64, 64}, {x, y, 64, 64}, {32, 32}, 0, WHITE);
}

bullet::SlowingBullet::SlowingBullet(turret::Turret* const &t, enemy::Enemy* const &e) : bullet::Bullet(t, e, 0.f) {}

bullet::SlowingBullet::~SlowingBullet() {} 


void bullet::SlowingBullet::Update()
{
    x = mTarget->x;
    y = mTarget->y;
    float parentCos = mParent->x + cosf(mParent->Rotation() * PI / 180.f) * 32.f, parentSin = mParent->y + sinf(mParent->Rotation() * PI / 180.f) * 32.f;
    float recLength = sqrtf(powf(x - parentCos, 2.f) + powf(y - parentSin, 2.f));
    DrawRectanglePro({parentCos, parentSin, recLength, 5.f}, {0}, mParent->Rotation(), BLUE);
}

void bullet::SlowingBullet::Draw(Texture2D& tileSheet) {}

bullet::ExplosiveBullet::ExplosiveBullet(turret::Turret* const &t, enemy::Enemy* const &e) : bullet::Bullet(t, e, 10.f) {}

bullet::ExplosiveBullet::~ExplosiveBullet() {} 


void bullet::ExplosiveBullet::Draw(Texture2D& tileSheet)
{
    DrawTexturePro(tileSheet, {1344, 640, 64, 64}, {x, y, 64, 64}, {32, 32}, mParent->Rotation() + 90.f, WHITE);
}