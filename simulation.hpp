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
    std::vector<Agent> singleRound(sf::RenderWindow &window, std::vector<Agent> &agentPop);
    void scoreAgents(std::vector<Agent> &agents);
    std::vector<Agent> initializePopulation(unsigned genNum);
};

#endif // SIMULATION_HPP
