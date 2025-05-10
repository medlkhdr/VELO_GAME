#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Clock.hpp>
#include <algorithm>
#include <cmath>

class Player {
public:
    static constexpr float MAX_STAMINA = 5.f;
    static constexpr float BOTTLE_STAMINA = 1.f;

    Player(const sf::Texture& texture);

    void resetPosition(const sf::Texture& roadTexture, float padLeft, float padRight,
                       int LANES, const sf::RenderWindow& window);

    void update(float dt, sf::Sound& tiredSound);

    void draw(sf::RenderWindow& window);

    void moveLeft();
    void moveRight(int LANES);

    sf::Sprite& getSprite() { return sprite; }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    float getStamina() const { return stamina; }
    float getWorldSpeed() const { return playerWorldSpeed; }
    int getLives() const { return lives; }
    void loseLife() { if (lives > 0) --lives; }

    void setLives(int newLives) { lives = newLives; }
    void setStamina(float newStamina) { stamina = newStamina; }

    bool isInvincible() const;
    void startInvincibility();

private:
    sf::Sprite sprite;
    sf::Sprite shadow;

    int playerLane = 1;
    int lives = 3; 
    float stamina = MAX_STAMINA;
    float playerWorldSpeed = DEFAULT_SPEED;
    bool triedWhileExhausted = false;

    bool invincible = false;
    sf::Clock invincibilityClock;
    static constexpr float INVINCIBILITY_DURATION = 2.0f;
    static constexpr float DEFAULT_SPEED = 4.f;
    static constexpr float MAX_SPEED = 12.f;
    static constexpr float ACCEL = 0.2f;
    static constexpr float BRAKE_FORCE = 0.5f;
    static constexpr float MIN_STAMINA_TO_BOOST = 2.0f;
    static constexpr float STAMINA_DRAIN = 6.f;
    static constexpr float STAMINA_REGEN = 2.0f;
};

#endif
