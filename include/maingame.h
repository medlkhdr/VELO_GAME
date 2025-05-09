#ifndef MAINGAME_H
#define MAINGAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "crono.h"
#include <string>
#include "player.h"
#include "collectables.h"
#include "eplayers.h"

class MainGame {
public:
    enum GameState { MENU, APROPOS, LOADING, GAME, HIT, FINISH };

    MainGame();
    ~MainGame();
    int run();
    

private:
    // Asset loading and UI
    bool loadAssets();
    void setupUI();
    void rebuildRoad();

    // Main loop functions
    void processEvents();
    void update(float dt);
    void render();

    // State-specific logic
    void updateLoading();
    void updateMenu();
    void updateAPropos();
    void updateGame();
    void updateHit();
    void updateFinish();

    // State-specific rendering
    void drawMenu();
    void drawAPropos();
    void drawLoading();
    void drawGame();
    void drawHit();
    void drawFinish();

    // Game logic
    void resetGame();

    // Window and state
    sf::RenderWindow window;
    GameState gameState;

    // Background and road
    sf::Texture roadTexture, grassTexture, bgTexture, finishLineTexture;
    sf::Sprite bgSprite, finishLine;
    float tileH;
    int roadTileCount;
    std::vector<sf::Sprite> roadTiles;
    static constexpr int NUM_TILES = 10;
    float RACE_DISTANCE;
    float FINISH_SPAWN_AT;
    bool finishLineSpawned;
    bool finishTriggered;
    bool raceFinished;
    // UI elements
    sf::Font font;
    sf::Text menu[4], shadow[4];
    sf::Text staminaLabel, positionLabel;
    sf::Text aproposText, aproposShadow;
    sf::Text finishTitle, finishScore, returnBtn;
    std::vector<std::vector<std::string>> aproposTexts;
    int selected;
    size_t currentTextIndex;

    // Audio
    sf::Music bgMusic;
    sf::SoundBuffer clickBuf, crashBuf, drinkBuf, coinBuf, tiredBuf, finishBuf;
    sf::Sound clickSound, crashSound, drinkSound, coinSound, tiredSound, finishSound;

    // Timing and clocks
    sf::Clock clockPulse;
    sf::Clock aproposScrollClock;
    sf::Clock loadingClock;
    sf::Clock fadeClock;
    sf::Clock deltaClock;
    sf::Clock finishTriggerClock;
    Crono crono;  // Timer for game duration

    // Environment assets
    std::vector<sf::Texture> treeTextures;
    std::vector<sf::Sprite> trees;
    std::vector<sf::Texture> eplayerTextures;
    sf::Texture playerTexture;

    // Collectibles
    sf::Texture bottleTex, coinTex;

    // Game entities
    Player* player;
    Collectables* collectables;
    EPlayers* eplayers;

    // Gameplay settings
    static constexpr float padLeft = 0.15f;
    static constexpr float padRight = 0.15f;
    static constexpr int LANES = 4;
    int score;
    float distanceTraveled;
};

#endif // MAINGAME_H
