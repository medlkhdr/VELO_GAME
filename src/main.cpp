#include "../include/maingame.h"
#include <cstdlib>
#include <ctime>

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    MainGame game;
    return game.run();
}