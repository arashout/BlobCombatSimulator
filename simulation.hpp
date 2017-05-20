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
    std::vector<Agent> initializePopulation(unsigned genNum, std::vector<Agent> winners);
    std::vector<Agent> propSelection(std::vector<Agent> &agents);
    std::vector<Agent> rankSelection(std::vector<Agent> &agents);

    void printScoreBoard(std::vector<Agent> &agents);
    void printDetails(std::vector<Agent> &agents);
    int simSpeed;
};

#endif // SIMULATION_HPP
