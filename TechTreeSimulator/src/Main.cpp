#include "controller/Game.h"
#include <iostream>

int main() {
    try {
        rm::controller::Game game;
        game.run();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
