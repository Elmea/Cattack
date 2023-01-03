#include "ressourceManager.hpp"
#include "ui.hpp"
#include <iostream>

ressourceManager::RessourceManager::RessourceManager()
{
    LoadTextures();
    LoadMusics();
    LoadSounds();
}

void ressourceManager::RessourceManager::LoadTextures()
{
    tileSheet = LoadTexture("assets/towerDefense_tilesheet.png");
    heart = LoadTexture("assets/heart.png");
    meowKnight = LoadTexture("assets/meowKnight/Meow-Knight_Run.png");
    meowAttack = LoadTexture("assets/meowKnight/Meow-Knight_Attack.png");
    meowDeath = LoadTexture("assets/meowKnight/Meow-Knight_Death.png");

    chikBoy = LoadTexture("assets/chikBoy/ChikBoy_run.png");
    chikBoyIdle = LoadTexture("assets/chikBoy/ChikBoy_idle.png");

    witchRun = LoadTexture("assets/witch/B_witch_run.png");
    witchDeath = LoadTexture("assets/witch/B_witch_death.png");
    witchAttack = LoadTexture("assets/witch/B_witch_attack.png");
    witchHealing = LoadTexture("assets/witch/B_witch_healing.png");

    explosionTexture = LoadTexture("assets/round_explosion/spritesheet.png");

    textureMainMenu = LoadTexture("assets/mainMenu.png");
    textureCredit = LoadTexture("assets/credit.png");
    textureLogoGalernois = LoadTexture("assets/GalernoisStudio.png");
}

void ressourceManager::RessourceManager::UnloadTextures()
{
    UnloadTexture(tileSheet);
    UnloadTexture(heart);

    UnloadTexture(meowKnight);
    UnloadTexture(meowAttack);
    UnloadTexture(meowDeath);

    UnloadTexture(chikBoy);
    UnloadTexture(chikBoyIdle);

    UnloadTexture(witchAttack);
    UnloadTexture(witchRun);
    UnloadTexture(witchHealing);
    UnloadTexture(witchDeath);

    UnloadTexture(explosionTexture);

    UnloadTexture(textureMainMenu);
    UnloadTexture(textureCredit);
    UnloadTexture(textureLogoGalernois);
}

void ressourceManager::RessourceManager::LoadSounds()
{
    boom = LoadSound("assets/sounds/boom.ogg");
    SetSoundVolume(boom, 0.5f);
    cheese = LoadSound("assets/sounds/cheese.ogg");
    droplet = LoadSound("assets/sounds/droplet.ogg");
    SetSoundVolume(droplet, 0.40f);
    heal = LoadSound("assets/sounds/heal.ogg");
    SetSoundVolume(heal, 0.35f);
    laser1 = LoadSound("assets/sounds/laser1.ogg");
    SetSoundVolume(laser1, 0.25f);
    laser2 = LoadSound("assets/sounds/laser2.ogg");
    SetSoundVolume(laser2, 0.25f);
    death = LoadSound("assets/sounds/enemy_death.ogg");
    launch = LoadSound("assets/sounds/launching.ogg");
    coin = LoadSound("assets/sounds/money_spend.ogg");
    SetSoundVolume(coin, 0.35f);
    gameOverSound = LoadSound("assets/sounds/game_over.ogg");
    SetSoundVolume(gameOverSound, 0.6f);
    gameOverSoundEaster = LoadSound("assets/sounds/(oof)wii-sports-theme-roblox-death-sound.ogg");
    SetSoundVolume(gameOverSoundEaster, 0.6f);
    swordAttack = LoadSound("assets/sounds/sword.ogg");
    musicMode = LoadSound("assets/sounds/musicModeEnable.ogg");
    upgradeSound = LoadSound("assets/sounds/upgrade.ogg");
}

void ressourceManager::RessourceManager::UnloadSounds()
{
    UnloadSound(boom);
    UnloadSound(cheese);
    UnloadSound(droplet);
    UnloadSound(heal);
    UnloadSound(laser1);
    UnloadSound(laser2);
    UnloadSound(death);
    UnloadSound(launch);
    UnloadSound(coin);
    UnloadSound(gameOverSound);
    UnloadSound(gameOverSoundEaster);
    UnloadSound(swordAttack);
    UnloadSound(musicMode);
    UnloadSound(upgradeSound);
}

void ressourceManager::RessourceManager::LoadMusics()
{
    credit = LoadMusicStream("assets/sounds/credit.ogg");
    for (int i = 0; i < 4; i++)
    {
        char musicPath[25] = "assets/sounds/music#.ogg";
        musicPath[19] = '0' + i + 1;
        musics[i] = LoadMusicStream(musicPath);
        SetMusicVolume(musics[i], 0.6f);
    }
}

void ressourceManager::RessourceManager::UnloadMusics()
{
    UnloadMusicStream(credit);
    for (int i = 0; i < 4; i++)
        UnloadMusicStream(musics[i]);
}

void ressourceManager::RessourceManager::MainMenuMusic()
{
    for (int i = 0; i < 4; i++)
        if (IsMusicStreamPlaying(musics[i]))
            StopMusicStream(musics[i]);

    if(!IsMusicStreamPlaying(credit))
        PlayMusicStream(credit);
    else
        UpdateMusicStream(credit);
}

void ressourceManager::RessourceManager::InGameMusic()
{
    if(IsMusicStreamPlaying(credit))
        StopMusicStream(credit);

    bool noMusicPlaying = true;
    for (int i = 0; i < 4; i++)
    {
        if(IsMusicStreamPlaying(musics[i]))
        {
            noMusicPlaying = false;
            break;
        }
    }

    if(noMusicPlaying)
        PlayMusicStream(musics[0]);
    
    for (int i = 0; i < 4; i++)
    {
        if (IsMusicStreamPlaying(musics[i])
            && GetMusicTimePlayed(musics[i]) < GetMusicTimeLength(musics[i]) - 1)
        {
            UpdateMusicStream(musics[i]);
            break;
        }
        else if (IsMusicStreamPlaying(musics[i])
            && GetMusicTimePlayed(musics[i]) > GetMusicTimeLength(musics[i]) - 1)
        {
            StopMusicStream(musics[i]);
            PlayMusicStream(musics[(i + 1) % 4]);
            break;   
        }
    }
}


void ressourceManager::RessourceManager::DrawAndUpdateVolume()
{
    ui::Slidder(10, 10, 100, 25, "Volume", mMasterVolume, WHITE);

    SetMasterVolume(mMasterVolume);
}