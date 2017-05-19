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
    Game g(mainWindow, agentMap);
    g.run();
}

int main()
{
    //Simulation s;
    gameTest();
    return 0;
}


