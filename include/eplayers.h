#ifndef EPLAYERS_H
#define EPLAYERS_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class EPlayers {
public:
    EPlayers(const std::vector<sf::Texture>& texs);

    void spawn(float roadTextureWidth,
               float padLeft, float padRight, int LANES,
               sf::RenderWindow& window);

    bool update(float obstacleSpeed,
                bool braking,
                sf::RenderWindow& window,
                sf::Sprite& player,
                sf::Sound& crashSound,
                int& lives,
                int& score);

    std::vector<sf::Sprite>&       getObstacles()       { return obstacles; }
    const std::vector<sf::Sprite>& getObstacles() const { return obstacles; }

    void clear() { obstacles.clear(); }

private:
    std::vector<sf::Texture> textures;
    std::vector<sf::Sprite>  obstacles;
};

#endif
