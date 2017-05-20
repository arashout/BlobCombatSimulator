#include <iostream>
#include "simulation.hpp"
#include <unordered_map>
#include "game.hpp"

void gameTest(void){
    std::unordered_map<std::string, Agent> agentMap;
    Agent a(0);
    a.setId("Player");
    agentMap.insert(std::make_pair(a.getId(), a));
    Agent b(0);
    agentMap.insert(std::make_pair(b.getId(), b));
    sf::RenderWindow mainWindow(sf::VideoMode(600, 600), "Blob Combat Simulator!");
    int gameSpeed = 1;
    Game g(mainWindow, agentMap);
    g.run(gameSpeed);
}

int main()
{
    // Declare and load a font
    sf::Font font;
    font.loadFromFile("/home/arash/Documents/BlobCombatSimulator/arial.ttf");

    //Simulation s;
    gameTest();
    return 0;
}


