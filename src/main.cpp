#include "../include/maingame.h"
#include <cstdlib>
#include <ctime>

int main() {
    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Create and run the game
    MainGame game;
    return game.run();
}