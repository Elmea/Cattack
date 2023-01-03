#include <math.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <raylib.h>

#include "level.hpp"
#include "ui.hpp"
#include "math/ourmath.hpp"
#include "bullet.hpp"

#include "game.hpp"

Game::Game()
{
    mCurrentLevel = 1;
    isLevelLoaded = false;
    isEnemiesInit = false;
    gameState = GameState::GM_INMENU;
}

Game::~Game()
{
    for(bullet::Bullet* b : bullets)
        delete b;
    for(turret::Turret* t : turrets)
        delete t;
    bullets.clear();
    enemies.clear();
    turrets.clear();
    explosions.clear();
}

void Game::LoadLevel(int lvl)
{
    int entry = -1;
    enemy::EnemyDir dir;
    const char *newLevel = Levels(entry, dir, lvl);

    map.ChangeMap(newLevel, entry, dir);
    isLevelLoaded = true;
}

void Game::Reset()
{
    itemSelected = -1;
    mCurrentLevel = 1;
    mWave = 1;
    map.tilesArray.clear();
    ResetTimer();
    ResetWaveTimer();
    ResetEnemiesTimer();
    money = 150;
    livesPlayer = 20;
    isLevelLoaded = false;
    isEnemiesInit = false;
    mEnemiesKilled = 0;
    enemiesToInitMin = 10;
    enemiesToInitMax = 15;
    mEnemiesToUpdate = 0;
    gamespeed = GameSpeed::GS_NORMAL;
    gameState = GameState::GM_INMENU;
    mWaveCounter = 0;
    mNbEnemiesSpawning= 0;
    enemiesCounter = 0;

    x1Button = GREEN;
    x2Button = GRAY;
    x4Button = GRAY;
    x8Button = GRAY;
    
    for(bullet::Bullet* b : bullets)
        delete b;
    for(turret::Turret* t : turrets)
        delete t;
    bullets.clear();
    enemies.clear();
    turrets.clear();
    explosions.clear();
}

void Game::ChangeSelectedItem(const float& zoneSize, const float& zoneX, const float& zoneY, const float& spacement)
{
    if(money - 50 >= 0 && ui::DragDropZone(zoneX, zoneY, zoneSize, zoneSize))
        itemSelected = -4;
    else if (money - 75 >= 0 && ui::DragDropZone(zoneX + spacement, zoneY, zoneSize, zoneSize))
        itemSelected = -3;
    else if (money - 150 >= 0 && ui::DragDropZone(zoneX + 2 * spacement, zoneY, zoneSize, zoneSize))
        itemSelected = -2;
}

void Game::DrawTurretCost(const float& zoneSize, const float& zoneX, const float& zoneY, const float& spacement)
{
    int basicTurretCost = 50, laserTurretCost = 75, rocketTurretCost = 150;
    DrawTexturePro(ressources.tileSheet, {1216, 640, 64, 64}, {zoneX, zoneY, zoneSize, zoneSize}, {0,0}, 0, WHITE);
    if (money - basicTurretCost >= 0)
        DrawText(TextFormat("£%d", basicTurretCost), zoneX, zoneY + zoneSize + 5, 20, YELLOW);
    else
        DrawText(TextFormat("£%d", basicTurretCost), zoneX, zoneY + zoneSize + 5, 20, RED);

    DrawTexturePro(ressources.tileSheet, {1216, 576, 64, 64}, {zoneX + spacement, zoneY, zoneSize, zoneSize}, {0,0}, 0, WHITE);
    if (money - laserTurretCost >= 0)
        DrawText(TextFormat("£%d", laserTurretCost), zoneX + spacement, zoneY + zoneSize + 5, 20, YELLOW);
    else
        DrawText(TextFormat("£%d", laserTurretCost), zoneX + spacement, zoneY + zoneSize + 5, 20, RED);

    DrawTexturePro(ressources.tileSheet, {1280, 512, 64, 64}, {zoneX + spacement*2, zoneY, zoneSize, zoneSize}, {0,0}, 0, WHITE);
    if (money - rocketTurretCost >= 0)
        DrawText(TextFormat("£%d", rocketTurretCost), zoneX + 2 * spacement, zoneY + zoneSize + 5, 20, YELLOW);
    else
        DrawText(TextFormat("£%d", rocketTurretCost), zoneX + 2 * spacement, zoneY + zoneSize + 5, 20, RED);
}

void Game::PlaceTurret(const int& mouseY, const int& tileX, const int& tileY)
{
    map::Tile& t = map.GetTile(tileX, tileY);
    if (t.IsFree() && !t.IsRoad() && mouseY < 720)
    {
        if (itemSelected == -4)
            turrets.push_back(new turret::ClassicTurret(tileX * TILES_SIZE + TILES_SIZE / 2, tileY * TILES_SIZE + TILES_SIZE / 2));
        else if (itemSelected == -3)
            turrets.push_back(new turret::GlueTurret(tileX * TILES_SIZE + TILES_SIZE / 2, tileY * TILES_SIZE + TILES_SIZE / 2));
        else if (itemSelected == -2)
            turrets.push_back(new turret::RocketTurret(tileX * TILES_SIZE + TILES_SIZE / 2, tileY * TILES_SIZE + TILES_SIZE / 2));
        money -= turrets[turrets.size() - 1]->GetCost();
        t.SetFree(false);
        PlaySound(ressources.coin);
    }
    itemSelected = -1;
}

// Draw current dragged turret at mouse position
void Game::DrawSelectedItem(const int& mouseX, const int& mouseY)
{
    if (itemSelected == -4)
    {
        DrawCircleLines(mouseX, mouseY, 150.f, BLACK);
        DrawTexturePro(ressources.tileSheet, {1216, 640, 64, 64}, {(float)mouseX - 25, (float)mouseY - 25, 50, 50}, {0,0}, 0, WHITE);
    } 
    else if (itemSelected == -3)
    {
        DrawCircleLines(mouseX, mouseY, 200.f, BLACK);
        DrawTexturePro(ressources.tileSheet, {1216, 576, 64, 64}, {(float)mouseX - 25, (float)mouseY - 25, 50, 50}, {0,0}, 0, WHITE);

    } 
    else if (itemSelected == -2)
    {
        DrawCircleLines(mouseX, mouseY, 125.f, BLACK);
        DrawTexturePro(ressources.tileSheet, {1280, 512, 64, 64}, {(float)mouseX - 25, (float)mouseY - 25, 50, 50}, {0,0}, 0, WHITE);
    }
}

void Game::DragDropManagement()
{
    if (gameState == GameState::GM_INMENU || gameState == GameState::GM_ISPAUSED)
        return;

    if (itemSelected >= -1)
    {
        float zoneSize = 50.f, zoneX = 10.f, zoneY = 780.f, spacement = 70.f;
        ChangeSelectedItem(zoneSize, zoneX, zoneY, spacement);
        DrawTurretCost(zoneSize, zoneX, zoneY, spacement);
    }
    else
    {
        int mouseX = GetMouseX(), mouseY = GetMouseY(), tileX = mouseX / TILES_SIZE, tileY = mouseY / TILES_SIZE;
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            PlaceTurret(mouseY, tileX, tileY);
        else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            DrawSelectedItem(mouseX, mouseY);
    }
}

void Game::InitEnemies()
{
    enemies.clear();
    enemiesCounter = GetRandomValue(enemiesToInitMin, enemiesToInitMax);
    if (mWave == 1)
    {
        for (int i = 0; i < enemiesCounter; i++)
        {
            enemies.emplace_back(std::make_unique<enemy::Basic>(&map.tilesArray[map.GetEntry()], map.GetFirstDir(), *this));
        }
    }
    else if (mWave < 3)
    {
        for (int i = 0; i < enemiesCounter; i++)
        {
            if (GetRandomValue(1, 6) >= 5)
                enemies.emplace_back(std::make_unique<enemy::Strong>(&map.tilesArray[map.GetEntry()], map.GetFirstDir(), *this));
            else
               enemies.emplace_back(std::make_unique<enemy::Basic>(&map.tilesArray[map.GetEntry()], map.GetFirstDir(), *this));
        } 
    }
    else if (mWave < 6)
    {
        for (int i = 0; i < enemiesCounter; i++)
        {
            if (GetRandomValue(1, 6) == 6)
                enemies.emplace_back(std::make_unique<enemy::Strong>(&map.tilesArray[map.GetEntry()], map.GetFirstDir(), *this));
            else if (GetRandomValue(1, 6) <= 2)
                enemies.emplace_back(std::make_unique<enemy::Healer>(&map.tilesArray[map.GetEntry()], map.GetFirstDir(), *this));
            else
                enemies.emplace_back(std::make_unique<enemy::Basic>(&map.tilesArray[map.GetEntry()], map.GetFirstDir(), *this));
        } 
    }
    else 
    {
        for (int i = 0; i < enemiesCounter; i++)
        {
            if (GetRandomValue(1, 6) >= 5)
                enemies.emplace_back(std::make_unique<enemy::Strong>(&map.tilesArray[map.GetEntry()], map.GetFirstDir(), *this));
            else if (GetRandomValue(1, 6) <= 2)
                enemies.emplace_back(std::make_unique<enemy::Healer>(&map.tilesArray[map.GetEntry()], map.GetFirstDir(), *this));
            else
                enemies.emplace_back(std::make_unique<enemy::Basic>(&map.tilesArray[map.GetEntry()], map.GetFirstDir(), *this));
        } 
    }
    isEnemiesInit = true;
    ResetTimer();
}

void Game::Draw()
{
    map.DrawTiles(ressources.tileSheet);
    for (turret::Turret *t : turrets)
    {
        t->Draw(ressources.tileSheet);
        DrawTexturePro(ressources.tileSheet, {64.f * t->GetLevel(), 768, 64, 64}, {t->x + 5, t->y + 5, 48, 48}, {0}, 0, YELLOW);
        if (t->GetLevel() < 3 && money >= t->GetCost() + t->GetLevel() * t->GetCost() / 3)
            DrawTexturePro(ressources.tileSheet, {832, 768, 64, 64}, {t->x + 5, t->y - 45, 48, 48}, {0}, 0, YELLOW);      
    }
    if (gameState != GameState::GM_WAVETIMER)
    {
        for (std::unique_ptr<enemy::Enemy>& e : enemies)
                e->Draw(*this);
    }
    for (bullet::Bullet *b : bullets)
        b->Draw(ressources.tileSheet);
    DrawRectangle(0, 720, SCREEN_WIDTH, 200, DARKGRAY);
    DrawText(TextFormat("£%d", money), SCREEN_WIDTH - 500, 780, 50, GREEN);
    DrawText(TextFormat("%d", livesPlayer), SCREEN_WIDTH - 250, 780, 50, RED);

    if(livesPlayer > 10)
        DrawTexturePro(ressources.heart,(Rectangle){0, 0, 260, 237},(Rectangle){SCREEN_WIDTH - 320, 780, 50, 50},(Vector2){0, 0}, 0, WHITE);
    else if(livesPlayer <= 10 && livesPlayer > 3)
        DrawTexturePro(ressources.heart,(Rectangle){260, 0, 260, 237},(Rectangle){SCREEN_WIDTH - 320, 780, 50, 50},(Vector2){0, 0}, 0, WHITE);
    else if(livesPlayer <= 3)
        DrawTexturePro(ressources.heart,(Rectangle){520, 0, 260, 237},(Rectangle){SCREEN_WIDTH - 320, 780, 50, 50},(Vector2){0, 0}, 0, WHITE);

    if (gameState != GameState::GM_ISWAITING && gameState != GameState::GM_MapSelection)
        DrawText(TextFormat("Wave : %d", mWave), SCREEN_WIDTH/2 - 50, 780, 50, WHITE);

    PlayExplosions();

    DrawFPS(10, 735);
}

void Game::ResetTimer()
{
    mStartTimer = 0;
}

void Game::ResetWaveTimer()
{
    mWaveTimer = 0;
}

void Game::ResetEnemiesTimer()
{
    mEnemiesTimer = 0;
}

void Game::IncrementTimers()
{
    mStartTimer += GetFrameTime();
    mEnemiesTimer += GetFrameTime();
    mWaveTimer += GetFrameTime();
}

void Game::ShowTimers()
{
    DrawText(TextFormat("startTimer : %f\nwaveTimer : %f\nenemiesTimer : %f", mStartTimer, mWaveTimer, mEnemiesTimer), 300, 800, 20, WHITE);
}

enemy::Enemy* Game::TurretAim(turret::Turret* t)
{
    float mostStepTaken = -1;
    enemy::Enemy* target = nullptr;
    for (std::unique_ptr<enemy::Enemy>& e : enemies)
    {
        unsigned int curStep = e->GetStep();
        if (e->isAlive && !e->Isleft && e->y <= 720 && e->y > 0 && e->x > 0 && e->x < SCREEN_WIDTH &&
            mostStepTaken < curStep && Distance2D((Vector2){(float)t->x, (float)t->y}, (Vector2){(float)e->x, (float)e->y}) < t->GetRange())
        {
            mostStepTaken = curStep;
            target = e.get();
        }
    }
    return target;
}

void Game::TurretFiring(turret::Turret* t, enemy::Enemy* target)
{
    if (dynamic_cast<turret::ClassicTurret *>(t))
    {
        PlaySound(ressources.droplet);
        bullets.push_back(new bullet::ClassicBullet(t, target));
    }
    else if (dynamic_cast<turret::GlueTurret *>(t))
    {
        mTieFighterSound = !mTieFighterSound;
        if (mTieFighterSound)
            PlaySound(ressources.laser1);
        else
            PlaySound(ressources.laser2);
        bullets.push_back(new bullet::SlowingBullet(t, target));
    }
    else if (dynamic_cast<turret::RocketTurret *>(t))
    {
        PlaySound(ressources.boom);
        bullets.push_back(new bullet::ExplosiveBullet(t, target));
    }
}

void Game::TurretManagement()
{
    time++;
    for (turret::Turret* currentTurret : turrets)
    {
        enemy::Enemy* target = TurretAim(currentTurret);
        if(!target)
            continue;
        
        currentTurret->Rotate(target);
        
        if(time % currentTurret->GetAttDelay() == 0)
            TurretFiring(currentTurret, target);
    }    
}

void Game::DeleteBullet(const int i)
{
    delete bullets[i];
    if ((int)bullets.size() > 1)
        bullets.erase(bullets.begin() + i);
    else
        bullets.clear();
}

void Game::BulletEffect(const int i, enemy::Enemy* target, const float distanceToTarget)
{
    if (dynamic_cast<bullet::SlowingBullet*>(bullets[i]) && distanceToTarget <= 2.f)
    {
        target->SetSlowTime(90);
        return;
    }

    bullet::ExplosiveBullet* expBull = dynamic_cast<bullet::ExplosiveBullet*>(bullets[i]);
    if (expBull && distanceToTarget <= 20.f)
    {
        AddExplosion(target->x, target->y);
        for (std::unique_ptr<enemy::Enemy>& e : enemies)
        {
            float distToTarget = Distance2D({target->x, target->y}, {e->x, e->y});
            if (e.get() != target && distToTarget < expBull->GetBlastRadius())
            {
                e->SetLife(e->GetLife() - bullets[i]->GetParent()->GetDamage() * (1.f - distToTarget/expBull->GetBlastRadius()));
                e->GetTouched();
            }
        }
    }
}

void Game::ApplyBulletDamageToTarget(const int i, enemy::Enemy* target, const float distanceToTarget)
{
    if (distanceToTarget <= 20.f)
    {
        target->GetTouched();
        target->SetLife(target->GetLife() - bullets[i]->GetParent()->GetDamage());
        DeleteBullet(i);
    }
}

void Game::BulletManagement()
{
    for (int i = 0; i < (int)bullets.size(); i++)
    {
        bullets[i]->Update();
        turret::Turret* parent = bullets[i]->GetParent();
        if (!parent)
            continue;

        // Delete if bullet is out of range
        if (Distance2D((Vector2){(float)bullets[i]->x, (float)bullets[i]->y}, (Vector2){(float)parent->x, (float)parent->y}) > parent->GetRange())
        {
            DeleteBullet(i);
            continue;
        }

        enemy::Enemy* target = bullets[i]->GetTarget();
        float distanceToTarget = Distance2D((Vector2){(float)bullets[i]->x, (float)bullets[i]->y}, (Vector2){(float)target->x, (float)target->y});
        
        if (!dynamic_cast<bullet::ClassicBullet*>(bullets[i]))        
            BulletEffect(i, target, distanceToTarget);
        
        ApplyBulletDamageToTarget(i, target, distanceToTarget);
    }
}

void Game::ShowdebugInfo()
{
    
    DrawText(TextFormat("Enemies Spawing : %d\nEnemies : %d", mNbEnemiesSpawning, enemiesCounter), 300, 740, 20, WHITE);
    
    ShowTimers();
    map.DrawFrame();
    DrawText(TextFormat("enemiesToInitMax : %d\nenemiesToInitMin : %d", enemiesToInitMax, enemiesToInitMin), 600, 850, 20, WHITE);
        
    for (map::Tile tile : map.tilesArray)
    {
        if (debug)
        {
            DrawText(tile.label, tile.WorldX() + 10, tile.WorldY() + 10, 12, BLACK);
            DrawText(TextFormat("%d", tile.GetId()), tile.WorldX() + 60, tile.WorldY() + 5, 10, BLACK);
        }
    }
}

void Game::WaveWaiting()
{
    mWaveWaitingTimer += GetFrameTime();
    DrawText(TextFormat("Next wave in %2.0f", 10 - mWaveWaitingTimer), SCREEN_WIDTH/2 - 70, 20, 20, WHITE);
    if (mWaveWaitingTimer >= 10.0f || IsKeyPressed(KEY_ENTER) || ui::Button(SCREEN_WIDTH/2 - 5, 40, 30, 20, 20, "Go", BLUE))
    {
        gameState = GameState::GM_ISRUNNING;
        mWaveWaitingTimer = 0;
        ResetWaveTimer();
        ResetEnemiesTimer();
        mEnemiesToUpdate = 0;
        enemiesToInitMax += 3;
        enemiesToInitMin += 2;
        PlaySound(ressources.launch);
    }
}

void Game::ClearDeadEnemies()
{
    int removed_enemies {0};
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [&removed_enemies](std::unique_ptr<enemy::Enemy>& enemy){
        
        if(!enemy->IsItDead())
            return false;

        removed_enemies++;
        return true;
    }), enemies.end());

    mEnemiesToUpdate -= removed_enemies;
    enemiesCounter   -= removed_enemies;
}

void Game::SelectATurret()
{
    int tileCenterX = GetMouseX() / TILES_SIZE * TILES_SIZE + TILES_SIZE / 2, tileCenterY = GetMouseY() / TILES_SIZE * TILES_SIZE + TILES_SIZE / 2;
    itemSelected = -1;
    for (int i = 0; i < (int) turrets.size(); i++)
        if (tileCenterX == turrets[i]->x && tileCenterY == turrets[i]->y)
            itemSelected = i;
}

void Game::RecycleTurret(const int refunds)
{
    money += refunds;
    map.GetTile(turrets[itemSelected]->x/TILES_SIZE, turrets[itemSelected]->y/TILES_SIZE).SetFree(true);

    for (int i = 0; i < (int) bullets.size(); i++)
        if (bullets[i]->GetParent() == turrets[itemSelected])
            DeleteBullet(i);

    delete turrets[itemSelected];
    if (turrets.size() > 1)
        turrets.erase(turrets.begin() + itemSelected);
    else
        turrets.clear();

    itemSelected = -1;
}

void Game::TurretActions()
{
    turrets[itemSelected]->Selected(money, ressources.upgradeSound);
    
    int refunds = 0;
    refunds += turrets[itemSelected]->GetCost();
    for (int i = 1; i < turrets[itemSelected]->GetLevel(); i++)
        refunds += turrets[itemSelected]->GetCost() + i * turrets[itemSelected]->GetCost() / 3;
    if (gameState != GameState::GM_ISWAITING)
        refunds = refunds * 80 / 100;

    DrawText(TextFormat("£%d refunded if sold", refunds), 1600/6 + 250, 870, 20, LIGHTGRAY);

    if(ui::Button(1600/6 + 250, 835, 110, 30, 25, "Recycle", RED))
        RecycleTurret(refunds);
}

void Game::TurretSelection()
{
    if (itemSelected >= -1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && GetMouseY() <= 720)
        SelectATurret();
    if(itemSelected >= 0)
        TurretActions();
}

void Game::Update()
{
    for (int i = 0; i < mEnemiesToUpdate; i++)
        enemies[i]->Update(*this);
    if (mEnemiesTimer >= 0.7f && mWaveTimer >= 5.0f)
    {
        if (mNbEnemiesSpawning == 0)
        {
            mNbEnemiesSpawning = GetRandomValue(1+mWave, enemies.size()/2);
        }
        if (mEnemiesToUpdate != (int)enemies.size())
        {
            mEnemiesToUpdate++;
            ResetEnemiesTimer();
            mWaveCounter++;
        }
        if (mWaveCounter == mNbEnemiesSpawning)
        {
            mWaveCounter = 0;
            mNbEnemiesSpawning = 0;
            ResetWaveTimer();
        }
    }

    if (enemiesCounter <= 0)
    {
        ResetTimer();
        mWave++;
        isEnemiesInit = false;
        gameState = GameState::GM_WAVETIMER;
        mWaveTimer = 0;
    }

    ClearDeadEnemies();
    TurretManagement();
    BulletManagement();

    IncrementTimers();
    DragDropManagement();
    TurretSelection();
}

void Game::MapSelection()
{
    if (IsKeyPressed(KEY_RIGHT) || ui::Button(SCREEN_WIDTH - 30, 450, '>', BLUE))
    {
        if (mCurrentLevel < 3)
        {           
            mCurrentLevel++;
            LoadLevel(mCurrentLevel);
        }
    }

    if (IsKeyPressed(KEY_LEFT) || ui::Button(30, 450, '<', BLUE))
    {
        if (mCurrentLevel > 1)
        {    
            mCurrentLevel--;
            LoadLevel(mCurrentLevel);
        }
    }
    
    DrawText(TextFormat("map %d", mCurrentLevel), SCREEN_WIDTH/2 - 10, 20, 20, WHITE);
    if (IsKeyPressed(KEY_ENTER) || ui::Button(SCREEN_WIDTH/2 - 50, 50, 100, 30, 30, "Select", BLUE))
    {
        gameState = GameState::GM_ISWAITING;
    }
}

void Game::IncrementEnemiesKilled()
{
    mEnemiesKilled++;
}

bool Game::UpdateAndDrawGameOver()
{
    // Draw
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorAlpha(GRAY, 0.3f));
    DrawText(TextFormat("YOU LOSE AT WAVE %d", GetWave()), SCREEN_WIDTH/4 - 100, SCREEN_HEIGHT/5, 100, BLACK);
    DrawText(TextFormat("You Killed %d enemies", GetEnemiesKilled()), SCREEN_WIDTH/4 - 100, SCREEN_HEIGHT/2, 100, BLACK);

    if (GetEnemiesKilled() == 0)
        DrawText("Noob !!", SCREEN_WIDTH/2 - 30, SCREEN_HEIGHT - 20, 20, BLACK);

    DrawText("Press enter to continue", SCREEN_WIDTH/2 - 80, 800, 20, WHITE);

    // Text input
    {
        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && mName.length() < NAME_MAX_LEN)
            {
                mName += key;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            if (mName.empty() == false)
                mName.pop_back();
        }

        Rectangle textBox {SCREEN_WIDTH/2 - 80, 700, 250, 60};
        DrawText("Please enter a name", textBox.x + 20, textBox.y - 30, 20, WHITE);
        DrawRectangleRec(textBox, LIGHTGRAY);
        DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, ORANGE);

        DrawText(mName.c_str(), (int)textBox.x + 5, (int)textBox.y + 8, 40, DARKGRAY);
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        mScoringTable.AddEntry(mName, mCurrentLevel, mWave, mEnemiesKilled);
        mScoringTable.WriteToFile();
        return true;
    }
    else
    {
        return false;
    }
}

// ============================= [LeaderBoard] =============================

bool CompareScore(const ScoreEntry& a, const ScoreEntry& b)
{
    if (a.wave > b.wave)
        return true;
    else if (a.wave < b.wave)
        return false;
    else
        return a.enemiesKilled > b.enemiesKilled;
}

Scoring::Scoring()
{
    std::ifstream myfile;
    myfile.open("scoring.bin", std::ios::binary);

    if (myfile.is_open())
    {
        myfile.read(reinterpret_cast<char*>(this), sizeof(Scoring));
        myfile.close();
    }
    else 
    {
        count = 0;
        for (int j = 0; j < MAX_SCORES; j++)
        {
            for (int i = 0; i < 10; i++)
                entries[j].name[i] = '\0';  
            entries[j].enemiesKilled = 0;
            entries[j].wave = 0;
            entries[j].map = 0;
        }        
    }
}

void Scoring::AddEntry(std::string& name, int map, int wave, int enemiesKilled)
{
    ScoreEntry entry = {};
    std::copy(name.begin(), name.end(), entry.name);
    entry.enemiesKilled = enemiesKilled;
    entry.wave = wave;
    entry.map = map;

    // Find index to insert
    int insertIndex;
    if (count == MAX_SCORES)
    {
        insertIndex = MAX_SCORES - 1;
    }
    else
    {
        insertIndex = count;
        count++;
    }

    // Insert it
    entries[insertIndex] = entry;

    // Reorder
    std::sort(entries, entries + MAX_SCORES, CompareScore);

}

void Scoring::WriteToFile()
{
    std::ofstream myfile("scoring.bin", std::ios::binary);
    if (myfile.is_open())
    {
        myfile.write(reinterpret_cast<char*>(this), sizeof(Scoring));
        myfile.close();
    }
}

void Game::DrawLeaderboard()
{
    DrawTexturePro(ressources.textureCredit, (Rectangle){1000, 0, 1100, 750}, (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, (Vector2){0, 0}, 0, WHITE);
    DrawRectangle(40, 30, SCREEN_WIDTH - 80, SCREEN_HEIGHT - 60, Fade(BLACK, 0.4));
    DrawText("Leaderboard", SCREEN_WIDTH/2 - 100, 30, 50, YELLOW);

    for (int i = 0; i < 10; i++)
        DrawText(TextFormat("[%d] %s : map : %d   wave : %d   Kill : %d", i+1, mScoringTable.entries[i].name, mScoringTable.entries[i].map, mScoringTable.entries[i].wave, mScoringTable.entries[i].enemiesKilled), 50, 80*(i+1), 50, WHITE);
    
}

// ============================= [Explosions] =============================

Explosion::Explosion(const float& inx, const float& iny)
{
    x = inx;
    y = iny;
}

void Game::AddExplosion(const float& x, const float& y)
{
    explosions.push_back(Explosion(x, y));
}

void Game::PlayExplosions()
{
    for (int i = 0; i < (int)explosions.size(); i++)
    {
        explosions[i].Play(ressources.explosionTexture);

        if (explosions[i].animationDone)
        {
            if ((int)explosions.size() > 1)
                explosions.erase(explosions.begin() + i);
            else
                explosions.clear();
        }
    }
}

void Explosion::Play(const Texture2D& tileSheet)
{
    mLastFrameRefresh += GetFrameTime();
    if (mLastFrameRefresh >= 0.025f)
    {
        mFrame++;
        mFrameX++;
        mLastFrameRefresh = 0;
    }

    Rectangle src = (Rectangle){100*(float)mFrameX, 100*(float)mFrameY, 100, 100};

    Rectangle dest = (Rectangle){x+50, y+50, 100, 100};

    if (mFrameX == 9)
    {
        mFrameY++;
        mFrameX = 0;
    }

    if (mFrame >= 70) 
    {
        mFrame = 0;
        animationDone = true;
    }
    
    DrawTexturePro(tileSheet, src, dest, (Vector2){100, 100}, 0, WHITE);
}

// Main loop

void Game::GameSpeedGestion()
{
    if (ui::Button(SCREEN_WIDTH - 200, 720, 50, 40, 40, "x1", x1Button) || IsKeyPressed(KEY_Q))
    {
        gamespeed = GameSpeed::GS_NORMAL;
        x1Button = GREEN;
        x2Button = GRAY;
        x4Button = GRAY;
        x8Button = GRAY;
    }

    if (ui::Button(SCREEN_WIDTH - 150, 720, 50, 40, 40, "x2", x2Button) || IsKeyPressed(KEY_W))
    {
        gamespeed = GameSpeed::GS_MEDFAST;
        x1Button = GRAY;
        x2Button = GREEN;
        x4Button = GRAY;
        x8Button = GRAY;
    }

    if (ui::Button(SCREEN_WIDTH - 100, 720, 50, 40, 40, "x4", x4Button) || IsKeyPressed(KEY_E))
    {
        gamespeed = GameSpeed::GS_TOOFAST;
        x1Button = GRAY;
        x2Button = GRAY;
        x4Button = GREEN;
        x8Button = GRAY;
    }

    if (ui::Button(SCREEN_WIDTH - 50, 720, 50, 40, 40, "x8", x8Button) || IsKeyPressed(KEY_E))
    {
        gamespeed = GameSpeed::GS_SONIC;
        x1Button = GRAY;
        x2Button = GRAY;
        x4Button = GRAY;
        x8Button = GREEN;
    }
}

void Game::MainLoop()
{
    if (menu.shouldDisplay && gameState == GameState::GM_INMENU)
    {
        if (menu.menuSelected != menu::MenuSelection::ME_GAMEOVER)
            ressources.MainMenuMusic();
        menu.Display(*this);
    }

    if (gameState != GameState::GM_INMENU)
    {
        ressources.InGameMusic();
        for (int i = 0; i < (int)gamespeed; i++)
        {                
            if (!isLevelLoaded)
            {
                LoadLevel(GetLevel());
            }
            else
            {
                Draw();

                if (gameState == GameState::GM_ISRUNNING || gameState == GameState::GM_ISPAUSED || gameState == GameState::GM_GAMEOVER)
                {
                    if (IsKeyPressed(KEY_F11))
                    {
                        livesPlayer = 0;
                    }
                    if (livesPlayer <= 0)
                    {
                        menu.menuSelected = menu::MenuSelection::ME_GAMEOVER;
                        gamespeed = GameSpeed::GS_NORMAL;
                        gameState = GameState::GM_INMENU;
                        menu.shouldDisplay = true;
                        menu.Display(*this);
                        if (GetEnemiesKilled() == 0)   
                            PlaySound(ressources.gameOverSoundEaster);
                        else
                            PlaySound(ressources.gameOverSound);
                    }
                    else if (gameState == GameState::GM_ISPAUSED || ui::Button(SCREEN_WIDTH - 250, 720, 40, 40, 40, "II", GRAY) || IsKeyPressed(KEY_P))
                    {
                        gameState = GameState::GM_ISPAUSED;
                        menu.shouldDisplay = true;
                        menu.menuSelected = menu::MenuSelection::ME_PAUSE;
                        menu.Display(*this);
                    }
                    else
                    {
                        if (isEnemiesInit)
                        {
                            Update();
                        }
                        else
                        {
                            InitEnemies();
                        }
                    }
                }
                else if (gameState == GameState::GM_WAVETIMER)
                {
                    WaveWaiting();
                    DragDropManagement();
                    TurretSelection();
                }
                else if (gameState == GameState::GM_ISWAITING)
                {
                    DrawText("or press space", SCREEN_WIDTH / 2 - 120, 680, 30, WHITE);
                    if (IsKeyPressed(KEY_SPACE) || ui::Button(SCREEN_WIDTH / 2 - 80, 630, 150, 50, 50, "Start", DARKBLUE))
                    {
                        gameState = GameState::GM_ISRUNNING;
                        ResetTimer();
                        ResetWaveTimer();
                        PlaySound(ressources.launch);
                    }
                    DragDropManagement();
                    TurretSelection();
                }
                else if (gameState == GameState::GM_MapSelection)
                {
                    MapSelection();
                }
                if (debug)
                {
                    ShowdebugInfo();
                }
                if (gameState == GameState::GM_ISRUNNING || gameState == GameState::GM_WAVETIMER)
                {
                    GameSpeedGestion();
                }
            }
        }
    }
    else // if we are not in game, show fps in the up right corner
    {
        DrawFPS(SCREEN_WIDTH - 80, 10);
    }
}

void Game::PlayIntro()
{
    bool intro = true;
    Color introCache = WHITE;

    while (intro)
    {
        BeginDrawing();

        DrawTexturePro(ressources.textureLogoGalernois, (Rectangle){0, 0, 1100, 750}, (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, (Vector2){0, 0}, 0, WHITE);
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, introCache);
        introCache.a--;
        if (introCache.a == 125u)
        {
            PlaySound(ressources.cheese);
        }
        if (introCache.a < 10u || IsKeyPressed(KEY_ENTER))
        {
            intro = false;
        }

        EndDrawing();
    }
}