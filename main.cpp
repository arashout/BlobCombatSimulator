#include <iostream>
#include "simulation.hpp"
#include <unordered_map>
#include "game.hpp"

sf::Font globalFont;

void gameTest(void){
    std::unordered_map<std::string, Agent> agentMap;
    Agent a(0);
    a.setId("Player");
    agentMap.insert(std::make_pair(a.getId(), a));
    Agent b(0);
    agentMap.insert(std::make_pair(b.getId(), b));
    Agent c(0);
    agentMap.insert(std::make_pair(c.getId(), c));
    sf::RenderWindow mainWindow(sf::VideoMode(600, 600), "Blob Combat Simulator!");
    sf::RenderWindow secondWindow(sf::VideoMode(300, 100), "Game Information");
    int gameSpeed = 1;
    Game g(mainWindow, agentMap, secondWindow);
    g.run(gameSpeed);
}

int main()
{
    globalFont.loadFromFile("/home/arash/Documents/BlobCombatSimulator/arial.ttf");
    //Simulation s;
    gameTest();
    return 0;
}


