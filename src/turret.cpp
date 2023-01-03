#include <raylib.h>

#include "ui.hpp"
#include "turret.hpp"
#include <math.h>

turret::Turret::Turret(const float& vX, const float& vY, const float& r, const int& del, const int& dmg, const int& c) : Entity(vX, vY), mRange(r), mDelayBetweenShell(del), mDamage(dmg), mCost(c) {}

turret::Turret::~Turret() {}

void turret::Turret::Selected(int& money, Sound& upgradeSound)
{
    DrawCircleLines(x, y, mRange, BLACK);
    DrawText(TextFormat("Level: %d", mLevel), 1600/6, 730, 20, LIGHTGRAY);
    DrawText(TextFormat("Damage: %d", mDamage), 1600/6, 770, 20, LIGHTGRAY);
    DrawText(TextFormat("Range: %.0f", mRange), 1600/6, 790, 20, LIGHTGRAY);
    DrawText(TextFormat("Reload: %.2fs", mDelayBetweenShell/60.f), 1600/6, 810, 20, LIGHTGRAY);
    if (mLevel < 3)    
        DrawText(TextFormat("Upgrade cost: £%d", mCost + mLevel * mCost / 3 ), 1600/6, 870, 20, LIGHTGRAY);
}

void turret::Turret::Rotate(enemy::Enemy* target)
{
    mRotation = atanf((target->y - y) / (target->x - x)) * 180.f / PI;
    if (target->x < x)
        mRotation += 180.f;
    else if (target->x > x && target->y > y)
        mRotation += 360.f;
}

float &turret::Turret::Rotation() { return mRotation; }

turret::ClassicTurret::ClassicTurret(const float& vX, const float& vY) : Turret(vX, vY, 150.f, 15, 10, 50) {}
turret::ClassicTurret::~ClassicTurret() {}

void turret::ClassicTurret::Draw(Texture2D &tileSheet)
{
    Rectangle src = (Rectangle){1216, 640, 64, 64};
    Rectangle dest = (Rectangle){x, y, 64, 64};
    DrawTexturePro(tileSheet, {1408, 448, 64, 64}, dest, (Vector2){32, 32}, 0, WHITE);
    DrawTexturePro(tileSheet, src, dest, (Vector2){32, 32}, mRotation + 90.f, WHITE);
}

void turret::ClassicTurret::Upgrade(int& funds)
{
    if (mLevel < 3)
    {
        mDamage += 5;
        mDelayBetweenShell -= 2;
    }
    funds -= mCost + mLevel * mCost / 3;
    mLevel++;
}

void turret::ClassicTurret::Selected(int& funds, Sound& upgradeSound)
{
    DrawText("Classic turret", 1600/6, 750, 20, LIGHTGRAY);
    turret::Turret::Selected(funds, upgradeSound);
    if(funds >= mCost + mLevel * mCost / 3 && mLevel < 3 && ui::Button(1600/6, 835, 110, 30, 25, "Upgrade", DARKGREEN))
    {
        Upgrade(funds);
        PlaySound(upgradeSound);
    }
}

turret::GlueTurret::GlueTurret(const float& vX, const float& vY) : Turret(vX, vY, 200.f, 30, 1, 75) {}

turret::GlueTurret::~GlueTurret() {}

void turret::GlueTurret::Draw(Texture2D &tileSheet)
{
    Rectangle src = (Rectangle){1216, 576, 64, 64};
    Rectangle dest = (Rectangle){x, y, 64, 64};
    DrawTexturePro(tileSheet, {1408, 448, 64, 64}, dest, (Vector2){32, 32}, 0, WHITE);
    DrawTexturePro(tileSheet, src, dest, (Vector2){32, 32}, mRotation + 90.f, WHITE);
}

void turret::GlueTurret::Upgrade(int& funds)
{
    mDamage += 2;
    mDelayBetweenShell -= 10;
    funds -= mCost + mLevel * mCost / 3;
    mLevel++;
}

void turret::GlueTurret::Selected(int& funds, Sound& upgradeSound)
{
    DrawText("Cryo turret", 1600/6, 750, 20, LIGHTGRAY);
    turret::Turret::Selected(funds, upgradeSound);
    if(funds >= mCost + mLevel * mCost / 3 && mLevel < 3 && ui::Button(1600/6, 835, 110, 30, 25, "Upgrade", DARKGREEN))
    {
        Upgrade(funds);
        PlaySound(upgradeSound);
    }
}

turret::RocketTurret::RocketTurret(const float& vX, const float& vY) : Turret(vX, vY, 125.f, 60, 30, 150) {}

turret::RocketTurret::~RocketTurret() {}

void turret::RocketTurret::Draw(Texture2D &tileSheet)
{
    Rectangle src = (Rectangle){1280, 512, 64, 64};
    Rectangle dest = (Rectangle){x, y, 64, 64};
    DrawTexturePro(tileSheet, {1408, 448, 64, 64}, dest, (Vector2){32, 32}, 0, WHITE);
    DrawTexturePro(tileSheet, src, dest, (Vector2){32, 32}, mRotation + 90.f, WHITE);
}

void turret::RocketTurret::Upgrade(int& funds)
{
    if (mLevel == 1)
        mDamage += 20;
    else if (mLevel == 2)
        mDamage += 25;
    funds -= mCost + mLevel * mCost / 3;
    mLevel++;
}

void turret::RocketTurret::Selected(int& funds, Sound& upgradeSound)
{
    DrawText("Rocket turret", 1600/6, 750, 20, LIGHTGRAY);
    turret::Turret::Selected(funds, upgradeSound);
    if(funds >= mCost + mLevel * mCost / 3 && mLevel < 3 && ui::Button(1600/6, 835, 110, 30, 25, "Upgrade", DARKGREEN))
    {
        Upgrade(funds);
        PlaySound(upgradeSound);
    }
}