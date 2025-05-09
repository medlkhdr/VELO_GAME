#include "../include/collectables.h"
#include <cstdlib>
#include <cmath>

Collectables::Collectables(const sf::Texture& bottleTexture, const sf::Texture& coinTexture)
: bottleTexture(bottleTexture), coinTexture(coinTexture)
{}

void Collectables::spawnBottle(float roadTextureWidth,
                              float padLeft, float padRight, int LANES,
                              sf::RenderWindow& window,
                              const std::vector<sf::Sprite>& obstacles,
                              const std::vector<sf::Sprite>& coins)
{
    if (std::rand() % 1000 < 5) {
        int lane = std::rand() % LANES;
        sf::Sprite b(bottleTexture);
        b.setScale(0.23f, 0.23f);  // Adjust bottle size as needed
        float rw = roadTextureWidth;
        float roadL = (window.getSize().x - rw) / 2.f;
        float lp = padLeft * rw;
        float inner = rw - (padLeft + padRight) * rw;
        float laneW = inner / LANES;
        float bx = roadL + lp + laneW * (lane + 0.5f) - b.getGlobalBounds().width / 2.f;
        float by = -b.getGlobalBounds().height - (std::rand() % 100);
        b.setPosition(bx, by);

        // Check for overlaps with existing bottles
        for (const auto& existingBottle : bottles) {
            if (std::abs(existingBottle.getPosition().y - b.getPosition().y) < 100.f)
                return;
        }
        
        // Check for overlaps with obstacles
        for (const auto& obs : obstacles) {
            if (b.getGlobalBounds().intersects(obs.getGlobalBounds()))
                return;
        }
        
        // Check for overlaps with coins
        for (const auto& coin : coins) {
            if (b.getGlobalBounds().intersects(coin.getGlobalBounds()))
                return;
        }
        
        bottles.push_back(b);
    }
}

void Collectables::spawnScoreCoin(float roadTextureWidth,
                                 float padLeft, float padRight, int LANES,
                                 sf::RenderWindow& window,
                                 const std::vector<sf::Sprite>& obstacles,
                                 const std::vector<sf::Sprite>& bottles)
{
    if (std::rand() % 1000 < 4) {
        int lane = std::rand() % LANES;
        sf::Sprite c(coinTexture);
        c.setScale(0.16f, 0.16f);  // Control the coin's size here
        float rw = roadTextureWidth;
        float roadL = (window.getSize().x - rw) / 2.f;
        float lp = padLeft * rw;
        float inner = rw - (padLeft + padRight) * rw;
        float laneW = inner / LANES;
        float cx = roadL + lp + laneW * (lane + 0.5f) - c.getGlobalBounds().width / 2.f;
        float cy = -c.getGlobalBounds().height - (std::rand() % 150);
        c.setPosition(cx, cy);

        // Check for overlaps with existing coins
        for (const auto& existingCoin : coins) {
            if (std::abs(existingCoin.getPosition().y - c.getPosition().y) < 100.f)
                return;
        }
        
        // Check for overlaps with obstacles
        for (const auto& obs : obstacles) {
            if (c.getGlobalBounds().intersects(obs.getGlobalBounds()))
                return;
        }
        
        // Check for overlaps with bottles
        for (const auto& bottle : bottles) {
            if (c.getGlobalBounds().intersects(bottle.getGlobalBounds()))
                return;
        }
        
        coins.push_back(c);
    }
}

void Collectables::updateBottles(float worldSpeed,
                               sf::RenderWindow& window,
                               sf::Sprite& player,
                               float& stamina,
                               const float MAX_STAMINA,
                               const float BOTTLE_STAMINA,
                               sf::Sound& drinkSound)
{
    for (auto it = bottles.begin(); it != bottles.end(); ) {
        it->move(0, worldSpeed);
        
        if (player.getGlobalBounds().intersects(it->getGlobalBounds())) {
            drinkSound.play();
            stamina = std::min(MAX_STAMINA, stamina + BOTTLE_STAMINA);
            it = bottles.erase(it);
        } 
        else if (it->getPosition().y > window.getSize().y) {
            it = bottles.erase(it);
        } 
        else {
            window.draw(*it);
            ++it;
        }
    }
}

void Collectables::updateCoins(float worldSpeed,
                             sf::RenderWindow& window,
                             sf::Sprite& player,
                             int& score,
                             sf::Sound& coinSound)
{
    for (auto it = coins.begin(); it != coins.end(); ) {
        it->move(0, worldSpeed);
        
        if (player.getGlobalBounds().intersects(it->getGlobalBounds())) {
            coinSound.play();
            score += 100;
            it = coins.erase(it);
        } 
        else if (it->getPosition().y > window.getSize().y) {
            it = coins.erase(it);
        } 
        else {
            window.draw(*it);
            ++it;
        }
    }
}