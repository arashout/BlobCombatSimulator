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
    std::vector<Agent> initializePopulation(unsigned genNum);
    std::vector<Agent> initializePopulation(unsigned genNum, Agent winner);
    Agent selection(std::vector<Agent> &agents);
};

#endif // SIMULATION_HPP
