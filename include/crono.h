#ifndef CRONO_H
#define CRONO_H
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Crono {
    private:
        sf::Clock      clock_;
        sf::Text       text_;
        float          DURATION;
    public:
        Crono(const sf::Font& font, unsigned characterSize = 24);
        void        restart();
        void        update();
        void        draw(sf::RenderTarget& target);
        bool        isFinished() const;
      float       getRemaining() const;
    };
#endif 