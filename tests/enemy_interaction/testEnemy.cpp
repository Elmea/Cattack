#include <raylib.h>
#include <vector>
#include <math.h>
#include <iostream>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

float distance2D(const Vector2 &a, const Vector2 &b) { return sqrtf(powf(a.x - b.x, 2.f) + powf(a.y - b.y, 2.f)); }
Vector2 lerp(Vector2 a, Vector2 b, float t)
{
    Vector2 result;
    result.x = (1.f - t) * a.x + t * b.x;
    result.y = (1.f - t) * a.y + t * b.y;
    return result;
}

class Enemy
{
private:
    int mHealAmount;

public:
    int life = 50;
    int mlifeMax = 100;
    Vector2 position;
    Vector2 direction = {1.f, 0.f};
    Enemy(Vector2 pos)
    {
        this->position = pos;
    }
    virtual void UpdateAndDraw()
    {
        Vector2 healthBar = lerp((Vector2){0, 5}, (Vector2){30, 5}, life * 1.0f / mlifeMax);

        DrawRectangleV((Vector2){position.x - 15, position.y - 20}, healthBar, GREEN);

        DrawRectangleLines(position.x - 15, position.y - 20, 30, 5, BLACK);
        DrawCircle(position.x, position.y, 10.f, RED);
        position = {position.x + direction.x, position.y + direction.y};
        if (position.x > SCREEN_WIDTH)
            position.x = 0.f;
    }
    int GetHeal()
    {
        mHealAmount = mlifeMax / 5;
        life += mHealAmount;
        if (life > mlifeMax)
            life = mlifeMax;
        return life;
    }
};

class BasicEnemy : public Enemy
{
private:
    int mHealAmount;

public:
    BasicEnemy(Vector2 pos) : Enemy(pos) {}
    virtual void UpdateAndDraw() override
    {
        Enemy::UpdateAndDraw();
        DrawText(TextFormat(" %02i", life), position.x, position.y - 20, 10, RED);
    }
};
class HealerEnemy : public Enemy
{
private:
    int mHealAmount;

public:
    int time = 0;
    HealerEnemy(Vector2 pos) : Enemy(pos){};
    virtual void UpdateAndDraw() override
    {
        Enemy::UpdateAndDraw();
        DrawCircle(position.x, position.y, 10.f, BLUE);
        DrawText(TextFormat(" %02i", life), position.x, position.y - 20, 10, BLUE);
    }
};
class Game
{
public:
    std::vector<Enemy *> enemy;
    int time = 0;
    int timer = 0;
    bool cooldown = false;
    Game()
    {
        enemy.push_back(new BasicEnemy((Vector2){130.f, 500.f}));
        enemy.push_back(new BasicEnemy((Vector2){180.f, 500.f}));
        enemy.push_back(new BasicEnemy((Vector2){50.f, 500.f}));
        enemy.push_back(new HealerEnemy((Vector2){0.f, 500.f}));
    }
    void UpdateAndDraw()
    {
        time++;
        timer = time % 240;
        for (Enemy *e : enemy)
        {

            HealerEnemy *h = dynamic_cast<HealerEnemy *>(e);
            if (h == NULL || timer != 0)
            {
                e->UpdateAndDraw();
                continue;
            }

            bool closeEnemy = false;

            for (Enemy *ene : enemy)
            {
                if (h == ene)
                    continue;

                if (distance2D(h->position, ene->position) < 40)
                {
                    ene->GetHeal();
                    closeEnemy = true;
                }
            }

            if (!closeEnemy)
                h->GetHeal();

            e->UpdateAndDraw();
        };
    };
};

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Enemy interaction");
    Game game;
    SetTargetFPS(60);
    // Main game loop
    while (!WindowShouldClose())
    {

        BeginDrawing();

        ClearBackground(RAYWHITE);
        game.UpdateAndDraw();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}