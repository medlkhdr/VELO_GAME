// crono.h
#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Crono {
    private:
        sf::Clock      clock_;
        sf::Text       text_;
        float          DURATION; // in seconds
    public:
        Crono(const sf::Font& font, unsigned characterSize = 24);
        void        restart();
        void        update();
        void        draw(sf::RenderTarget& target);
        bool        isFinished() const;
   
       // Returns how many whole seconds remain (0 … DURATION)
      float       getRemaining() const;

    };
