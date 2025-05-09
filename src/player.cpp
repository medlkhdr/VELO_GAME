#include "../include/player.h"
#include <SFML/Window/Keyboard.hpp>

Player::Player(const sf::Texture& texture)
{
    sprite.setTexture(texture);
    sprite.setScale(0.25f, 0.25f);
    shadow = sprite;
    shadow.setScale(0.20f, 0.20f);
    shadow.setColor(sf::Color(0, 0, 0, 150));
}

void Player::resetPosition(const sf::Texture& roadTexture,
                           float padLeft, float padRight,
                           int LANES,
                           const sf::RenderWindow& window)
{
    float rw = static_cast<float>(roadTexture.getSize().x);
    float roadLeft = (window.getSize().x - rw) / 2.f;
    float leftPad = padLeft * rw;
    float inner = rw - (padLeft + padRight) * rw;
    float laneW = inner / LANES;
    float pw = sprite.getGlobalBounds().width;
    float playerTargetX = roadLeft + leftPad + laneW * (playerLane + 0.5f) - pw / 2.f;
    float py = window.getSize().y - sprite.getGlobalBounds().height - 10.f;
    sprite.setPosition(playerTargetX, py);

    // Update shadow position
    shadow.setPosition(sprite.getPosition().x + 5.f, sprite.getPosition().y + 5.f);
}

void Player::update(float dt, sf::Sound& tiredSound) {
    // 1) Read inputs
    bool boosting = sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    bool braking  = sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

    // 2) Handle boosting
    if (boosting) {
        if (stamina >= MIN_STAMINA_TO_BOOST) {
            // Drain stamina & accelerate
            stamina = std::max(0.f, stamina - STAMINA_DRAIN * dt);
            playerWorldSpeed = std::min(playerWorldSpeed + ACCEL, MAX_SPEED);
            triedWhileExhausted = false;
        } else if (!triedWhileExhausted) {
            // Play tired sound once when you can't boost
            tiredSound.play();
            triedWhileExhausted = true;
        }
    }
    // 3) Handle braking
    else if (braking) {
        // Instant stop
        playerWorldSpeed = 0.f;
        triedWhileExhausted = false;
    }
    // 4) Idle (neither boost nor brake)
    else {
        // Regenerate stamina
        stamina = std::min(MAX_STAMINA, stamina + STAMINA_REGEN * dt);
        // Ease speed back toward DEFAULT_SPEED
        if (playerWorldSpeed < DEFAULT_SPEED)
            playerWorldSpeed = std::min(playerWorldSpeed + ACCEL, DEFAULT_SPEED);
        else if (playerWorldSpeed > DEFAULT_SPEED)
            playerWorldSpeed = std::max(playerWorldSpeed - BRAKE_FORCE, DEFAULT_SPEED);
        triedWhileExhausted = false;
    }

    // 5) Update shadow to follow
    shadow.setPosition(sprite.getPosition() + sf::Vector2f(5.f, 5.f));
}


// Invincibility methods
bool Player::isInvincible() const {
    if (invincible &&
        invincibilityClock.getElapsedTime().asSeconds() >= INVINCIBILITY_DURATION) {
        const_cast<Player*>(this)->invincible = false;
    }
    return invincible;
}

void Player::startInvincibility() {
    invincible = true;
    invincibilityClock.restart();
}

void Player::draw(sf::RenderWindow& window) {
    // Draw shadow first
    window.draw(shadow);

    // Blink effect when invincible
    if (isInvincible()) {
        float t = invincibilityClock.getElapsedTime().asSeconds();
        uint8_t alpha = (static_cast<int>(t * 10) % 2 == 0) ? 100 : 255;
        sprite.setColor(sf::Color(255, 255, 255, alpha));
    } else {
        sprite.setColor(sf::Color::White);
    }

    window.draw(sprite);
}

void Player::moveLeft() {
    if (playerLane > 0) --playerLane;
}

void Player::moveRight(int LANES) {
    if (playerLane < LANES - 1) ++playerLane;
}