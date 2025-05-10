// crono.cpp
#include "../include/crono.h"
#include <cmath>
#include <iomanip>
#include <sstream>

Crono::Crono(const sf::Font& font, unsigned characterSize)
: text_()
{
    text_.setFont(font);
    text_.setCharacterSize(characterSize);
    text_.setFillColor(sf::Color::White);
    text_.setOutlineColor(sf::Color::Black);
    text_.setOutlineThickness(2.f);
    restart();
    DURATION = 60.f;  
}

void Crono::restart()
{
    clock_.restart();
}

void Crono::update()
{
}

void Crono::draw(sf::RenderTarget& target)
{
    float elapsed = clock_.getElapsedTime().asSeconds();
    float remaining = std::max(0.f, DURATION - elapsed);

    std::ostringstream ss;
    ss << std::floor(remaining);
    text_.setString("Time: " + ss.str());

    auto bounds = text_.getLocalBounds();
    sf::Vector2f pos(target.getSize().x - bounds.width - 20.f,
                     20.f);
    text_.setPosition(pos);

    target.draw(text_);
}

bool Crono::isFinished() const
{
    return clock_.getElapsedTime().asSeconds() >= DURATION;
}

float Crono::getRemaining() const
{
    float rem = DURATION - clock_.getElapsedTime().asSeconds();
    return rem > 0.f ? rem : 0.f;
}