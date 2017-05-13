#include <iostream>
#include "game.hpp"

int main()
{
    srand((unsigned int) time(0));
    // create the window
    sf::RenderWindow window(sf::VideoMode(600, 600), "Blob Combat Simulator!");

    Game game(window);
    game.run();
    return 0;
}
