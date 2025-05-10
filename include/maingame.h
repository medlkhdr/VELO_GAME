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
    bool loadAssets();
    void setupUI();
    void rebuildRoad();

    void processEvents();
    void update(float dt);
    void render();

    void updateLoading();
    void updateMenu();
    void updateAPropos();
    void updateGame();
    void updateHit();
    void updateFinish();

    void drawMenu();
    void drawAPropos();
    void drawLoading();
    void drawGame();
    void drawHit();
    void drawFinish();
    void resetGame();

    sf::RenderWindow window;
    GameState gameState;
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

    sf::Music bgMusic;
    sf::SoundBuffer clickBuf, crashBuf, drinkBuf, coinBuf, tiredBuf, finishBuf;
    sf::Sound clickSound, crashSound, drinkSound, coinSound, tiredSound, finishSound;

    sf::Clock clockPulse;
    sf::Clock aproposScrollClock;
    sf::Clock loadingClock;
    sf::Clock fadeClock;
    sf::Clock deltaClock;
    sf::Clock finishTriggerClock;
    Crono crono;  

    std::vector<sf::Texture> treeTextures;
    std::vector<sf::Sprite> trees;
    std::vector<sf::Texture> eplayerTextures;
    sf::Texture playerTexture;

    sf::Texture bottleTex, coinTex;

    Player* player;
    Collectables* collectables;
    EPlayers* eplayers;

    static constexpr float padLeft = 0.15f;
    static constexpr float padRight = 0.15f;
    static constexpr int LANES = 4;
    int score;
    float distanceTraveled;
};

#endif
