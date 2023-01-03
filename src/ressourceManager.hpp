#pragma once
#include <raylib.h>

namespace ressourceManager
{
    class RessourceManager
    {
    private:
        float mMasterVolume = 1.0f;
        void LoadTextures();
        void LoadSounds();
        void LoadMusics();

    public:
        RessourceManager();
        RessourceManager(const RessourceManager& copy) = delete;
        RessourceManager& operator=(const RessourceManager& copy) = delete;

        // ==== [Texture] ====
        Texture2D tileSheet;
        Texture2D heart;

        Texture2D meowKnight;
        Texture2D meowAttack;
        Texture2D meowDeath;

        Texture2D chikBoy;
        Texture2D chikBoyIdle;

        Texture2D witchRun;
        Texture2D witchDeath;
        Texture2D witchAttack;
        Texture2D witchHealing;

        Texture2D explosionTexture;

        Texture2D textureMainMenu;
        Texture2D textureCredit;
        Texture2D textureLogoGalernois;
        
        // ==== [Sound] ====
        Sound boom;
        Sound cheese;
        Sound droplet;
        Sound heal;
        Sound laser1;
        Sound laser2;
        Sound death;
        Sound launch;
        Sound coin;
        Sound gameOverSound;
        Sound gameOverSoundEaster;
        Sound swordAttack;
        Sound musicMode;
        Sound upgradeSound;

        // ==== [Music] ====
        Music credit;
        Music musics[4];

        void MainMenuMusic();
        void InGameMusic();

        // ==== [Tools] ====
        void DrawAndUpdateVolume();
        void UnloadSounds();
        void UnloadMusics();
        void UnloadTextures();
    };
}

