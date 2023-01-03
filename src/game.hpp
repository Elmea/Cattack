#pragma once
#include <string>
#include <memory>

#include "map.hpp"
#include "eBasic.hpp"
#include "eHealer.hpp"
#include "eStrong.hpp"
#include "turret.hpp"
#include "bullet.hpp"
#include "menu.hpp"
#include "ressourceManager.hpp"

#define MAX_SCORES 10
#define NAME_MAX_LEN 10

struct ScoreEntry
{
    char name[NAME_MAX_LEN+1];
    int map;
    int wave;
    int enemiesKilled;
};

struct Scoring
{
    int count;
    ScoreEntry entries[MAX_SCORES];

    Scoring();
    void AddEntry(std::string& str, int map, int wave, int enemiesKilled);
    void WriteToFile();
};

enum class GameState
{
    GM_INMENU,
    GM_ISPAUSED,
    GM_ISWAITING,
    GM_ISRUNNING,
    GM_WAVETIMER,
    GM_MapSelection,
    GM_GAMEOVER
};

enum class GameSpeed : int
{
    GS_NORMAL = 1,
    GS_MEDFAST = 2,
    GS_TOOFAST = 4,
    GS_SONIC = 8,
};

class Explosion
{
private:
    float x;
    float y;
    float mLastFrameRefresh = 0;
    int mFrame = 0;
    int mFrameX = 0;
    int mFrameY = 0;

public:
    bool animationDone = false;
    void Play(const Texture2D& tileSheet);
    Explosion(const float& x, const float& y);
};

class Game
{
private:
    int mCurrentLevel;
    float mStartTimer = 0;
    float mEnemiesTimer = 0;
    float mWaveTimer = 0;
    int mWave = 1;

    int mEnemiesToUpdate = 0;
    int mWaveCounter = 0;
    int mNbEnemiesSpawning= 0;

    float mWaveWaitingTimer = 0;
    int mEnemiesKilled = 0;
    Scoring mScoringTable;

    std::string mName;

    bool mTieFighterSound = false;
    void ChangeSelectedItem(const float& zoneSize, const float& zoneX, const float& zoneY, const float& spacement);
    void DrawTurretCost(const float& zoneSize, const float& zoneX, const float& zoneY, const float& spacement);
    void PlaceTurret(const int& mouseY, const int& tileX, const int& tileY);
    void DrawSelectedItem(const int& mouseX, const int& mouseY);
    
    enemy::Enemy* TurretAim(turret::Turret* t);
    void TurretFiring(turret::Turret* t, enemy::Enemy* target);
    void DeleteBullet(const int i);
    void BulletEffect(const int i, enemy::Enemy* target, const float distanceToTarget);
    void ApplyBulletDamageToTarget(const int i, enemy::Enemy* target, const float distanceToTarget);

    void SelectATurret();
    void TurretActions();
    void RecycleTurret(const int refunds);

    Color x1Button = GREEN;
    Color x2Button = GRAY;
    Color x4Button = GRAY;
    Color x8Button = GRAY;

    menu::Menu menu;

public:
    Game();
    ~Game();

    bool debug = false;
    int money = 150;

    int livesPlayer = 20;
    int enemiesToInitMin = 10;
    int enemiesToInitMax = 15;

    int enemiesCounter = 0;

    unsigned int time = 0;

    // ==== [Tool] ====

    int itemSelected = -1;
    bool isLevelLoaded = false;
    bool isEnemiesInit;
    GameState gameState;
    
    std::vector<std::unique_ptr<enemy::Enemy>> enemies;
    std::vector<turret::Turret*> turrets;
    std::vector<bullet::Bullet*> bullets;
    std::vector<Explosion> explosions;

    map::Map map;

    GameSpeed gamespeed = GameSpeed::GS_NORMAL;

    ressourceManager::RessourceManager ressources;

    int GetLevel() const { return mCurrentLevel; }
    void LoadLevel(const int lvl);

    void Reset();
    void Update();
    void Draw();

    void InitEnemies();

    void ResetTimer();
    void ResetEnemiesTimer();
    void ResetWaveTimer();

    void IncrementTimers();

    float GetTimer() const { return mStartTimer; }
    float GetWaveTimer() const { return mEnemiesTimer; }
    float GetEnemiesTimer() const { return mWaveTimer; }

    void ShowTimers();

    void TurretManagement();
    void BulletManagement();
    void DragDropManagement();
    void TurretSelection();

    void ShowdebugInfo();

    void WaveWaiting();
    void MapSelection();
    void ClearDeadEnemies();

    int GetWave() const { return mWave; }
    int GetEnemiesKilled() const { return mEnemiesKilled; }
    void IncrementEnemiesKilled();

    bool UpdateAndDrawGameOver(); // Return true at end

    void DrawLeaderboard();
    bool CompareScore(const ScoreEntry& a, const ScoreEntry& b);

    void AddExplosion(const float& x, const float& y);
    void PlayExplosions();

    void GameSpeedGestion();
    void PlayIntro();
    void MainLoop();
};

