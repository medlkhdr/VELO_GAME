#ifndef EPLAYERS_H
#define EPLAYERS_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class EPlayers {
public:
    // Constructor: take a list of textures for your opponent sprites
    EPlayers(const std::vector<sf::Texture>& texs);

    // Spawn new obstacle if conditions met
    void spawn(float roadTextureWidth,
               float padLeft, float padRight, int LANES,
               sf::RenderWindow& window);

    // Move & draw obstacles; reverse direction when braking.
    //   obstacleSpeed = player->getWorldSpeed()
    //   braking       = whether brake key is down
    //   window        = render target
    //   player        = the player sprite (for collision)
    //   crashSound    = sound to play on hit
    //   lives         = reference to player's lives (decremented on hit)
    //   score         = reference to score (+=10 when obstacle passes)
    // Returns true if a collision occurred this frame.
    bool update(float obstacleSpeed,
                bool braking,
                sf::RenderWindow& window,
                sf::Sprite& player,
                sf::Sound& crashSound,
                int& lives,
                int& score);

    // Accessors for the obstacle list
    std::vector<sf::Sprite>&       getObstacles()       { return obstacles; }
    const std::vector<sf::Sprite>& getObstacles() const { return obstacles; }

    // Clear all obstacles (used in resetGame)
    void clear() { obstacles.clear(); }

private:
    std::vector<sf::Texture> textures;
    std::vector<sf::Sprite>  obstacles;
};

#endif // EPLAYERS_H
