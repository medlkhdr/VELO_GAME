#ifndef COLLECTABLES_H
#define COLLECTABLES_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class Collectables {
public:
    Collectables(const sf::Texture& bottleTexture, const sf::Texture& coinTexture);

    void spawnBottle(float roadTextureWidth,
                     float padLeft, float padRight, int LANES,
                     sf::RenderWindow& window,
                     const std::vector<sf::Sprite>& obstacles,
                     const std::vector<sf::Sprite>& coins);

    void spawnScoreCoin(float roadTextureWidth,
                        float padLeft, float padRight, int LANES,
                        sf::RenderWindow& window,
                        const std::vector<sf::Sprite>& obstacles,
                        const std::vector<sf::Sprite>& bottles);

    void updateBottles(float worldSpeed,
                       sf::RenderWindow& window,
                       sf::Sprite& player,
                       float& stamina,
                       const float MAX_STAMINA,
                       const float BOTTLE_STAMINA,
                       sf::Sound& drinkSound);

    void updateCoins(float worldSpeed,
                     sf::RenderWindow& window,
                     sf::Sprite& player,
                     int& score,
                     sf::Sound& coinSound);

    std::vector<sf::Sprite>& getBottles() { return bottles; }
    const std::vector<sf::Sprite>& getBottles() const { return bottles; }
    
    std::vector<sf::Sprite>& getCoins() { return coins; }
    const std::vector<sf::Sprite>& getCoins() const { return coins; }

    void clear() {
        bottles.clear();
        coins.clear();
    }

private:
    std::vector<sf::Sprite> bottles;
    std::vector<sf::Sprite> coins;
    sf::Texture bottleTexture;
    sf::Texture coinTexture;
};

#endif