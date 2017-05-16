#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <unordered_map>
#include "game.hpp"
#include "agent.hpp"

class Simulation
{
public:
    Simulation();
private:
    void singleGame(std::unordered_map<std::string, Agent> &batchAgents);
    void singleRound(std::unordered_map<std::string, Agent> &currentPopulation);

    std::unordered_map<std::string, Agent> initializePopulation(unsigned genNum);

    std::unordered_map<std::string, Agent> agentPopulation;
};

#endif // SIMULATION_HPP
