#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <unordered_map>
#include <vector>
#include "game.hpp"
#include "agent.hpp"

class Simulation
{
public:
    Simulation();
private:
    void singleGame(std::unordered_map<std::string, Agent> &batchAgents, sf::RenderWindow &window);
    void singleRound(sf::RenderWindow &window);
    void scoreRound(void);
    void initializePopulation(unsigned genNum);


    std::vector<Agent> agentPopulation;
};

#endif // SIMULATION_HPP
