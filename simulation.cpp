#include "simulation.hpp"
#include "game.hpp"
#include "parameters.hpp"
#include <iostream>
#include <algorithm>


Simulation::Simulation() {
    // Set random seed
    std::srand((unsigned int) time(0));

    sf::RenderWindow mainWindow(sf::VideoMode(600, 600), "Blob Combat Simulator!");
    // Initialize generation 0 population
    std::vector<Agent> currentPopulation = initializePopulation(0);
    for(size_t i = 1; i <= simParams::numGenerations; i++){
        std::vector<Agent> testedAgentPopulation = singleRound(mainWindow, currentPopulation);
        currentPopulation = initializePopulation(i, selection(testedAgentPopulation));
        std::cout << "Generation: " << std::to_string(i) << std::endl;
    }
}

std::vector<Agent> Simulation::singleRound(sf::RenderWindow &window, std::vector<Agent> &agentPop){
    std::unordered_map<std::string, Agent> currentBatch;

    std::vector<Agent> testedPopulation;
    unsigned counter = 1;

    for(auto &a : agentPop){
        // TODO - PASS BY REFERENCE SOMEHOW!
        // Then I won't have to re-create the vector
        currentBatch.insert(std::make_pair(a.getId(), a));

        if(counter % simParams::batchSize == 0){
            singleGame(currentBatch, window);
            // Re-create agent vector
            for(auto &kv : currentBatch){
                Agent &agent = kv.second;
                testedPopulation.push_back(agent);
            }

            currentBatch.clear();
        }
        counter++;
    }
    return testedPopulation;
}

void Simulation::singleGame(std::unordered_map<std::string, Agent> &batchAgents,
                            sf::RenderWindow &window){
    Game game(window, batchAgents);
    game.run();
}

std::vector<Agent> Simulation::initializePopulation(unsigned genNum){
    std::vector<Agent> agents;
    for(size_t i = 0; i < (simParams::numBatches * simParams::batchSize); i++){
        Agent a(genNum);
        agents.push_back(a);
    }
    return agents;
}

std::vector<Agent> Simulation::initializePopulation(unsigned genNum, Agent winner)
{
    std::vector<Agent> agents;
    for(size_t i = 0; i < (simParams::numBatches * simParams::batchSize); i++){
        Agent a(genNum, winner);
        a.mutate();
        agents.push_back(a);
    }
    return agents;
}

/**
 * @brief Select an agent to pass on it's traits
 * @param agents
 */
Agent Simulation::selection(std::vector<Agent> &agents){
    std::sort(agents.begin(), agents.end());
    std::reverse(agents.begin(), agents.end());
    float totalFitness = 0;
    for(size_t i = 0; i < agents.size(); i++){
        Agent &a = agents[i];
        totalFitness += a.computeFitness();
    }
    // random value between 0 and totalFitness
    float randVal = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/totalFitness));

    float selectionSum = 0;
    unsigned selectionIndex;
    for(size_t i = 0; i < agents.size(); i++){
        selectionSum += agents[i].computeFitness();
        if(selectionSum > randVal){
            selectionIndex = i;
            break;
        }
    }
    return agents[selectionIndex];
}
