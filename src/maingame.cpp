#include "../include/maingame.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

MainGame::MainGame()
: window(sf::VideoMode(1920,1080), "Bike Game", sf::Style::Default)
, gameState(MENU)
, tileH(0.f), roadTileCount(0)
, finishLineSpawned(false), finishTriggered(false), raceFinished(false)
, selected(0), currentTextIndex(0)
, treeTextures(5), eplayerTextures(5)
, player(nullptr), collectables(nullptr), eplayers(nullptr)
, score(0), distanceTraveled(0.f)
, crono(font, 24)
{
    window.setFramerateLimit(60);
    RACE_DISTANCE = 0.f;
    FINISH_SPAWN_AT = 0.f;

    if (!loadAssets()) {
        std::cerr << "Error loading assets\n";
        window.close();
    }
    setupUI();
    rebuildRoad();

    // Initialize game entities
    player      = new Player(playerTexture);
    collectables = new Collectables(bottleTex, coinTex);
    eplayers = new EPlayers(eplayerTextures);
    player->resetPosition(roadTexture, padLeft, padRight, LANES, window);
}

MainGame::~MainGame() {
    delete player;
    delete collectables;
    delete eplayers;
}

int MainGame::run()
{
    while (window.isOpen())
	{
        float dt = deltaClock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
    return 0;
}

sf::Texture gameOverTexture ;
sf::Sprite gameOverSprite ;
bool MainGame::loadAssets()
{
    if (!font.loadFromFile("resources/fonts/Pixelite.ttf")) return false;
    if (!bgTexture.loadFromFile("resources/images/bgmenu.jpg")) return false;
    bgSprite = sf::Sprite(bgTexture);

    if (!roadTexture.loadFromFile("resources/images/road.png")) return false;
    tileH = static_cast<float>(roadTexture.getSize().y);
    RACE_DISTANCE = tileH * NUM_TILES;
    FINISH_SPAWN_AT = RACE_DISTANCE - 500.f;

    if (!finishLineTexture.loadFromFile("resources/images/finish.png")) return false;
    finishLine.setTexture(finishLineTexture);

    grassTexture.loadFromFile("resources/images/grass.png");
    grassTexture.setRepeated(true);

    // Load the bike/player texture
   if (!playerTexture.loadFromFile("resources/images/player.png"))
       return false;
   playerTexture.setSmooth(true);

    // Load tree textures
    for (int i = 1; i < 5; ++i) {
        std::string p = "resources/images/trees/tree" + std::to_string(i) + ".png";
        treeTextures[i].loadFromFile(p);
    }
    if (!gameOverTexture.loadFromFile("resources/images/gameover.png"))
        return false;
    gameOverSprite.setTexture(gameOverTexture);
    // Load obstacle textures
    for (int i = 0; i < 5; ++i)
	{
        std::string p = "resources/images/obstacles/eplayer" + std::to_string(i+1) + ".png";
        eplayerTextures[i].loadFromFile(p);
    }
    // Load collectible textures
    bottleTex.loadFromFile("resources/images/coins/bottle.png");
    coinTex.loadFromFile("resources/images/coins/score.png");

    // Load audio files
    bgMusic.openFromFile("resources/audios/bgmenu.ogg");
    bgMusic.setLoop(true); 
    bgMusic.setVolume(25.f); 
    bgMusic.play();

    clickBuf.loadFromFile("resources/audios/click.wav");
    crashBuf.loadFromFile("resources/audios/crash.wav");
    drinkBuf.loadFromFile("resources/audios/drink.wav");
    coinBuf.loadFromFile("resources/audios/coin.wav");
    tiredBuf.loadFromFile("resources/audios/tired.wav");
    finishBuf.loadFromFile("resources/audios/finish.wav");

    clickSound.setBuffer(clickBuf);
    crashSound.setBuffer(crashBuf);
    drinkSound.setBuffer(drinkBuf);
    coinSound.setBuffer(coinBuf);
    tiredSound.setBuffer(tiredBuf);
    finishSound.setBuffer(finishBuf);
	std::cout << "Game Over Texture Size: "
          << gameOverTexture.getSize().x << "x"
          << gameOverTexture.getSize().y << std::endl;
    return true;
}

void MainGame::setupUI() {
    std::string labels[4] = {"Jouer", "A propos", "Quitter", "RETOUR"};
    for (int i = 0; i < 4; i++) {
        menu[i].setFont(font);
        menu[i].setString(labels[i]);
        menu[i].setCharacterSize(32);
        menu[i].setFillColor(sf::Color::White);
        shadow[i] = menu[i];
        shadow[i].setFillColor(sf::Color::Black);
    }

    positionLabel.setFont(font);
    positionLabel.setCharacterSize(24);
    positionLabel.setFillColor(sf::Color::White);
    positionLabel.setString("VOTRE POSITION :");

    finishTitle.setFont(font);
    finishTitle.setCharacterSize(64);
    finishTitle.setFillColor(sf::Color::Yellow);
    
    finishScore.setFont(font);
    finishScore.setCharacterSize(32);
    finishScore.setFillColor(sf::Color::White);
    
    returnBtn.setFont(font);
    returnBtn.setCharacterSize(28);
    returnBtn.setString("RETOUR AU MENU");
    returnBtn.setFillColor(sf::Color::White);

    aproposTexts = {{
        "Bienvenue dans notre projet de mini-jeu de velo",
        "Realise par Mahmoud Moukouch & Mohamed Lakhdar",
        "Encadre par Professeur Rachida Hannane",
        "Dans notre filiere IAPS4 a l'Universite FSSM Marrakech",
        "Ce jeu est conçu pour offrir une experience immersive",
        "Avec des graphismes futuristes et un gameplay dynamique",
        "Le but est de collecter des objets tout en evitant des obstacles",
        "- Collecte de bouteilles pour gagner des points",
        "- Evitez les autres velos sur la route",
        "- Profitez de l'adrenaline d'une course a grande vitesse",
        "- Compteur de score pour suivre vos progres",
        "- Limite de temps pour rendre le defi encore plus excitant",
        "Nous esperons que vous apprecierez ce jeu innovant!",
        "Merci de jouer et bonne chance!"
    }};
    
    aproposText = sf::Text("", font, 28);
    aproposText.setFillColor(sf::Color::White);
    
    aproposShadow = aproposText;
    aproposShadow.setFillColor(sf::Color::Black);
}

void MainGame::rebuildRoad() {
    float winH    = static_cast<float>(window.getSize().y);
    tileH         = static_cast<float>(roadTexture.getSize().y);
    roadTileCount = static_cast<int>(std::ceil(winH / tileH)) + 1;

    float startY = (tileH > winH ? 0.f : winH - roadTileCount * tileH);

    float winW   = static_cast<float>(window.getSize().x);
    float rw     = static_cast<float>(roadTexture.getSize().x);
    float roadLeft = (winW > rw ? (winW - rw) * 0.5f : 0.f);

    roadTiles.clear();
    roadTiles.reserve(roadTileCount);
    for (int i = 0; i < roadTileCount; ++i) {
        sf::Sprite t(roadTexture);
        t.setPosition(roadLeft, startY + i * tileH);
        roadTiles.push_back(std::move(t));
    }
}



void MainGame::processEvents() {
    sf::Event ev;
    while (window.pollEvent(ev)) {
        if (ev.type == sf::Event::Closed) {
            window.close();
        }
        else if (ev.type == sf::Event::Resized) {
            window.setView(sf::View(sf::FloatRect(0, 0, ev.size.width, ev.size.height)));
            rebuildRoad();
            player->resetPosition(roadTexture, padLeft, padRight, LANES, window);
        }
        else if (ev.type == sf::Event::KeyPressed) {
            if (gameState == MENU) {
                if (ev.key.code == sf::Keyboard::Up)   selected = (selected + 2) % 3;
                if (ev.key.code == sf::Keyboard::Down) selected = (selected + 1) % 3;
                if (ev.key.code == sf::Keyboard::Enter) {
                    clickSound.play();
                    if (selected == 0) { 
                        resetGame();
                        gameState = LOADING; 
                        loadingClock.restart(); 
                    }
                    else if (selected == 1) { 
                        gameState = APROPOS; 
                        aproposScrollClock.restart(); 
                        currentTextIndex = 0; 
                    }
                    else window.close();
                }
            }
            else if (gameState == APROPOS && ev.key.code == sf::Keyboard::Enter) {
                clickSound.play(); 
                gameState = MENU; 
                selected = 0;
            }
            else if (gameState == FINISH && ev.key.code == sf::Keyboard::Enter) {
                clickSound.play();
                gameState = MENU;
                selected = 0;
            }
            else if ((gameState == GAME || gameState == HIT) &&
                   (ev.key.code == sf::Keyboard::A || ev.key.code == sf::Keyboard::Left)) {
                player->moveLeft();
                player->resetPosition(roadTexture, padLeft, padRight, LANES, window);
            }
            else if ((gameState == GAME || gameState == HIT) &&
                   (ev.key.code == sf::Keyboard::D || ev.key.code == sf::Keyboard::Right)) {
                player->moveRight(LANES);
                player->resetPosition(roadTexture, padLeft, padRight, LANES, window);
            }
        }
    }
}

void MainGame::update(float dt) {
    switch (gameState) {
                case LOADING:   updateLoading();   break;
                case MENU:           break;
                case APROPOS:   updateAPropos();   break;
                case GAME:
                    updateGame();
                    break;
                case HIT:
                    updateGame();
                    updateHit();
                    break;
                case FINISH:    updateFinish();    break;
            }
}

void MainGame::render() {
    window.clear();
    switch (gameState) {
        case MENU:    drawMenu();    break;
        case APROPOS: drawAPropos(); break;
        case LOADING: /* Drawing handled in updateLoading */ break;
        case GAME:
        case HIT:     drawGame();    break;
        case FINISH:  drawFinish();  break;
    }
}

void MainGame::resetGame() {
    finishLineSpawned = false;
    finishTriggered = false;
    raceFinished = false;
    score = 0;
    distanceTraveled = 0.f;
    
    collectables->clear();
    eplayers->getObstacles().clear();
    trees.clear();
    
    player->resetPosition(roadTexture, padLeft, padRight, LANES, window);
}

void MainGame::updateLoading() {
    window.clear(sf::Color::Black);
    sf::Text txt("Chargement en cours...", font, 30);
    int alpha = static_cast<int>(127.5f * (std::sin(clockPulse.getElapsedTime().asSeconds() * 2 * 3.1415f) + 1));
    txt.setFillColor(sf::Color(255, 255, 0, alpha));
    txt.setPosition(window.getSize().x/2.f - txt.getGlobalBounds().width/2.f,
                    window.getSize().y/2.f);
    window.draw(txt);
    window.display();
    
    if (loadingClock.getElapsedTime().asSeconds() > 3.f) {
        fadeClock.restart();
        gameState = GAME;
    }
}


void MainGame::updateAPropos() {
    float scrollY = window.getSize().y + 40.f
                    - aproposScrollClock.getElapsedTime().asSeconds() * 60.f;
    if (scrollY + aproposTexts[currentTextIndex].size() * 40.f < -100.f) {
        currentTextIndex = (currentTextIndex + 1) % aproposTexts.size();
        aproposScrollClock.restart();
    }
}

void MainGame::updateHit() {
    if (player->getLives() <= 0)
	{
        player->setLives(3);
        gameState = MENU;
        return;
    }
    if (!player->isInvincible()) {
        gameState = GAME;
    }
}

void MainGame::updateGame() {
    if (crono.isFinished()) {
            clickSound.play();
            resetGame();                       gameState = MENU;     
            return;                
        }
    crono.update();
    float dt = deltaClock.restart().asSeconds();
    player->update(dt, tiredSound);

    bool braking    = sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
                      sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    float baseSpeed = player->getWorldSpeed();
    float worldSpeed   = braking ? 0.f : baseSpeed;
    float eplayerSpeed = braking ? -baseSpeed : baseSpeed;

    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);
    float rw   = static_cast<float>(roadTexture.getSize().x);
    float roadLeft = (winW - rw) * 0.5f;

    for (auto& t : roadTiles) {
        t.move(0, worldSpeed);
        if (t.getPosition().y >= winH) {
            t.setPosition(roadLeft, t.getPosition().y - roadTileCount * tileH);
        } else {
            t.setPosition(roadLeft, t.getPosition().y);
        }
    }

    if (!finishLineSpawned && distanceTraveled >= FINISH_SPAWN_AT) {
        float scale = rw / float(finishLineTexture.getSize().x);
        finishLine.setScale(scale, scale);
        finishLine.setPosition(roadLeft, -finishLine.getGlobalBounds().height);
        finishLineSpawned = true;
    }
    if (finishLineSpawned) {
        finishLine.move(0, worldSpeed);
        if (!finishTriggered && player->getBounds().intersects(finishLine.getGlobalBounds())) {
            finishTriggered = true;
            finishTriggerClock.restart();
            finishSound.play();
        }
        if (finishTriggered &&
            finishTriggerClock.getElapsedTime().asSeconds() >= 2.f) {
            gameState = FINISH;
        }
    }

    if (std::rand() % 100 < 2 &&
       (trees.empty() || trees.back().getPosition().y > 200.f)) {
        sf::Sprite tr(treeTextures[1 + std::rand() % (treeTextures.size()-1)]);
        float tw = tr.getGlobalBounds().width,
              th = tr.getGlobalBounds().height;
        bool leftSide = (std::rand() % 2) == 0;
        float tx = leftSide
            ? std::rand() % int(roadLeft - tw + 1)
            : roadLeft + rw
              + std::rand() % int(winW - (roadLeft + rw) - tw + 1);
        tr.setPosition(tx, -th);
        trees.push_back(tr);
    }
    for (auto it = trees.begin(); it != trees.end();) {
        it->move(0, worldSpeed);
        if (it->getPosition().y > winH)
            it = trees.erase(it);
        else
            ++it;
    }

    eplayers->spawn(rw, padLeft, padRight, LANES, window);
    int newLives = player->getLives();
    bool collided = eplayers->update(
        eplayerSpeed,
        braking,
        window,
        player->getSprite(),
        crashSound,
        newLives,
        score
    );
    if (collided && !player->isInvincible()) {
        player->setLives(newLives);
        crashSound.play();
        player->startInvincibility();
        fadeClock.restart();
        gameState = (player->getLives() <= 0 ? MENU : HIT);
    }

    collectables->spawnBottle(
        rw, padLeft, padRight, LANES,
        window, eplayers->getObstacles(), collectables->getCoins()
    );
    float stamina = player->getStamina();
    collectables->updateBottles(
        worldSpeed, window, player->getSprite(),
        stamina, Player::MAX_STAMINA, Player::BOTTLE_STAMINA, drinkSound
    );
    player->setStamina(stamina);

    collectables->spawnScoreCoin(
        rw, padLeft, padRight, LANES,
        window, eplayers->getObstacles(), collectables->getBottles()
    );
    collectables->updateCoins(
        worldSpeed, window, player->getSprite(),
        score, coinSound
    );

    distanceTraveled += baseSpeed;
}



void MainGame::updateFinish() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        clickSound.play();
        gameState = MENU;
    }
}
void MainGame::drawMenu() {
    window.draw(bgSprite);
    float cx = window.getSize().x/2.f,
          startY = window.getSize().y/2.f - 80.f;
    int alpha = static_cast<int>(127.5f * (std::sin(clockPulse.getElapsedTime().asSeconds() * 2 * 3.1415f) + 1));
    
    for (int i = 0; i < 3; i++) {
        auto b = menu[i].getLocalBounds();
        float x = cx - (b.width/2.f + b.left),
              y = startY + i * 60.f - b.top;
        shadow[i].setPosition(x+2, y+2);
        menu[i].setPosition(x, y);
        if (i == selected) {
            menu[i].setFillColor(sf::Color(255, 255, 0, alpha));
            shadow[i].setFillColor(sf::Color(0, 0, 0, alpha));
        } else {
            menu[i].setFillColor(sf::Color::White);
            shadow[i].setFillColor(sf::Color::Black);
        }
        window.draw(shadow[i]);
        window.draw(menu[i]);
    }
    window.display();
}

void MainGame::drawAPropos() {
    window.draw(bgSprite);
    float scrollY = window.getSize().y + 40.f
                    - aproposScrollClock.getElapsedTime().asSeconds() * 60.f;
    float cx = window.getSize().x/2.f;
    
    for (size_t i = 0; i < aproposTexts[currentTextIndex].size(); i++) {
        aproposText.setString(aproposTexts[currentTextIndex][i]);
        float px = cx - aproposText.getGlobalBounds().width/2.f,
              py = scrollY + i * 40.f;
        aproposShadow.setString(aproposTexts[currentTextIndex][i]);
        aproposShadow.setPosition(px+2, py+2);
        aproposText.setPosition(px, py);
        if (py > -50 && py < window.getSize().y-80) {
            window.draw(aproposShadow);
            window.draw(aproposText);
        }
    }
    
    shadow[3].setPosition(window.getSize().x/2.f - (menu[3].getLocalBounds().width/2.f
                          + menu[3].getLocalBounds().left) + 2,
                          window.getSize().y - 60.f + 2);
    menu[3].setPosition(window.getSize().x/2.f - (menu[3].getLocalBounds().width/2.f
                        + menu[3].getLocalBounds().left),
                        window.getSize().y - 60.f);
    window.draw(shadow[3]);
    window.draw(menu[3]);
    window.display();
}

void MainGame::drawLoading() {
    window.clear(sf::Color::Black);
    sf::Text txt("Chargement en cours...", font, 30);
    int alpha = static_cast<int>(127.5f * (std::sin(clockPulse.getElapsedTime().asSeconds() * 2 * 3.1415f) + 1));
    txt.setFillColor(sf::Color(255, 255, 0, alpha));
    txt.setPosition(window.getSize().x/2.f - txt.getGlobalBounds().width/2.f,
                    window.getSize().y/2.f);
    window.draw(txt);
}

void MainGame::drawGame() {
    // 1) Clear
    window.clear();

    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);
    float rw   = static_cast<float>(roadTexture.getSize().x);

    // 2) Full-screen scrolling grass background
    static float grassOffset = 0.f;
    grassOffset = std::fmod(grassOffset - player->getWorldSpeed(),
                            static_cast<float>(grassTexture.getSize().y));
    if (grassOffset < 0) grassOffset += grassTexture.getSize().y;
    sf::RectangleShape grassBg({ winW, winH });
    grassBg.setTexture(&grassTexture);
    grassBg.setTextureRect({
        0,
        static_cast<int>(grassOffset),
        static_cast<int>(winW),
        static_cast<int>(winH)
    });
    window.draw(grassBg);

    // 3) Compute roadLeft clamped ≥ 0
    float roadLeft = (winW > rw ? (winW - rw) * 0.5f : 0.f);

    // 4) Draw & wrap road tiles
    for (auto& t : roadTiles) {
        t.setPosition(roadLeft, t.getPosition().y);
        window.draw(t);
    }

    // 5) Finish line
    if (finishLineSpawned) {
        window.draw(finishLine);
    }

    // 6) Trees
    for (auto& tr : trees) {
        window.draw(tr);
    }

    for (auto& obs : eplayers->getObstacles()) {
        window.draw(obs);
    }

    for (auto& b : collectables->getBottles()) window.draw(b);
    for (auto& c : collectables->getCoins())   window.draw(c);

    player->draw(window);

    const float BAR_W = 20.f, BAR_H = 150.f;
    float barX = winW - BAR_W - 60.f;
    float barY = (winH - BAR_H) * 0.7f;
    staminaLabel.setPosition(barX - staminaLabel.getGlobalBounds().width - 20.f, barY - staminaLabel.getCharacterSize());
    window.draw(staminaLabel);
    sf::RectangleShape bgBar({ BAR_W, BAR_H });
    bgBar.setPosition(barX, barY);
    bgBar.setFillColor(sf::Color(50,50,50,200));
    window.draw(bgBar);
    float fillH = (player->getStamina() / Player::MAX_STAMINA) * BAR_H;
    sf::RectangleShape fgBar({ BAR_W, fillH });
    fgBar.setPosition(barX, barY + (BAR_H - fillH));
    fgBar.setFillColor(sf::Color(100,100,255,200));
    window.draw(fgBar);

    const float PB_W = 300.f, PB_H = 15.f;
    float progress = std::min(1.f, distanceTraveled / RACE_DISTANCE);
    float pbX = (winW - PB_W) * 0.5f;
    float pbY = winH - PB_H - 10.f;
    positionLabel.setPosition(pbX, pbY - positionLabel.getCharacterSize() - 5.f);
    window.draw(positionLabel);
    sf::RectangleShape pbg({ PB_W, PB_H });
    pbg.setPosition(pbX, pbY);
    pbg.setFillColor(sf::Color(50,50,50,200));
    window.draw(pbg);
    sf::RectangleShape pfg({ PB_W * progress, PB_H });
    pfg.setPosition(pbX, pbY);
    pfg.setFillColor(sf::Color(100,255,100,220));
    window.draw(pfg);

    sf::Text hud(
        "Score: " + std::to_string(score) +
        "  Lives: " + std::to_string(player->getLives()),
        font, 24
    );
    hud.setFillColor(sf::Color::White);
    hud.setPosition(20.f, 20.f);
    window.draw(hud);

    int secondsLeft = static_cast<int>(crono.getRemaining());
    sf::Text timeText(
        "TIME : " + std::to_string(secondsLeft),
        font, 24
    );
    timeText.setFillColor(sf::Color::White);
    float hudRight = hud.getPosition().x
                    + hud.getLocalBounds().left
                    + hud.getLocalBounds().width;
    timeText.setPosition(hudRight + 20.f, hud.getPosition().y);
    window.draw(timeText);

    window.display();
}





void MainGame::drawHit() {
    drawGame();
    
    // Add flashing effect when hit
    static sf::Clock flashClock;
    if (static_cast<int>(flashClock.getElapsedTime().asMilliseconds() / 100) % 2 == 0) {
        sf::RectangleShape flash(sf::Vector2f(window.getSize().x, window.getSize().y));
        flash.setFillColor(sf::Color(255, 0, 0, 100)); // Semi-transparent red
        window.draw(flash);
    }
    
    window.display();
}

void MainGame::drawFinish() {
    sf::Text ret = returnBtn, retSh = returnBtn;
    
    // Position the return button
    auto bb = ret.getLocalBounds();
    ret.setPosition(window.getSize().x/2.f - (bb.width/2.f + bb.left),
                    window.getSize().y * 0.6f + 250.f);
    retSh.setPosition(ret.getPosition() + sf::Vector2f(2, 2));

    // Animate the return button
    int alpha = static_cast<int>(127.5f * (std::sin(clockPulse.getElapsedTime().asSeconds() * 2 * 3.1415f) + 1));
    ret.setFillColor(sf::Color(255, 255, 0, alpha));
    retSh.setFillColor(sf::Color(0, 0, 0, alpha));

    window.clear();

    // Show congratulations message
    finishTitle.setString("FELICITATIONS!");
    finishTitle.setPosition(window.getSize().x/2.f - (finishTitle.getLocalBounds().width/2.f
                            + finishTitle.getLocalBounds().left),
                            window.getSize().y * 0.2f);
    window.draw(finishTitle);

    // Show final score
    finishScore.setString("Votre score est " + std::to_string(score));
    finishScore.setPosition(window.getSize().x/2.f - (finishScore.getLocalBounds().width/2.f
                            + finishScore.getLocalBounds().left),
                            window.getSize().y * 0.4f + 80.f);
    window.draw(finishScore);

    // Draw return button
    window.draw(retSh);
    window.draw(ret);

    window.display();
}
