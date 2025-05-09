#include "../include/eplayers.h"
#include <cstdlib>

EPlayers::EPlayers(const std::vector<sf::Texture>& texs)
: textures(texs)
{}

void EPlayers::spawn(float roadTextureWidth,
                     float padLeft, float padRight, int LANES,
                     sf::RenderWindow& window)
{
    // Spawn new obstacles with some probability (10%) if previous ones moved far enough
    if (std::rand() % 100 < 10 &&
        (obstacles.empty() || obstacles.back().getPosition().y > 150.f))
    {
        int lane = std::rand() % LANES;
        sf::Sprite obs(textures[std::rand() % textures.size()]);
        obs.setScale(0.20f, 0.20f);

        float rw = roadTextureWidth;
        float roadLeft = (window.getSize().x - rw) / 2.f;
        float laneW = (rw - (padLeft + padRight) * rw) / LANES;
        float cx_lane = roadLeft + padLeft * rw + laneW * (lane + 0.5f);

        obs.setPosition(
            cx_lane - obs.getGlobalBounds().width / 2.f,
            -obs.getGlobalBounds().height - (std::rand() % 101 + 50)
        );
        obstacles.push_back(obs);
    }
}

bool EPlayers::update(float obstacleSpeed,
                      bool braking,
                      sf::RenderWindow& window,
                      sf::Sprite& player,
                      sf::Sound& crashSound,
                      int& lives,
                      int& score)
{
    bool collided = false;
    float winH = static_cast<float>(window.getSize().y);

    // Reverse direction when braking
    float speed = braking ? -obstacleSpeed : obstacleSpeed;

    for (auto it = obstacles.begin(); it != obstacles.end(); )
    {
        // Move obstacle
        it->move(0, speed);

        // Reduced collision boxes
        sf::FloatRect pb = player.getGlobalBounds();
        pb.left   += pb.width * 0.25f; pb.width  *= 0.5f;
        sf::FloatRect ob = it->getGlobalBounds();
        ob.left   += ob.width * 0.25f; ob.width  *= 0.5f;

        // Collision?
        if (pb.intersects(ob))
        {
            crashSound.play();
            --lives;
            collided = true;
            it = obstacles.erase(it);
        }
        // Passed off bottom
        else if (it->getPosition().y > winH)
        {
            it = obstacles.erase(it);
            score += 10;
        }
        else
        {
            // Draw shadow + obstacle
            sf::Sprite sh = *it;
            sh.move(5.f, 5.f);
            sh.setColor(sf::Color(0, 0, 0, 150));
            window.draw(sh);
            window.draw(*it);
            ++it;
        }
    }

    return collided;
}
