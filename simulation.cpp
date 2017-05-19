#include "simulation.hpp"
#include "game.hpp"
#include "parameters.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>


Simulation::Simulation() {
    // Set random seed
    std::srand((unsigned int) time(0));

    sf::RenderWindow mainWindow(sf::VideoMode(600, 600), "Blob Combat Simulator!");
    // Initialize generation 0 population
    std::vector<Agent> currentPopulation = initializePopulation(0);
    for(size_t i = 1; i <= simParams::numGenerations; i++){
        std::vector<Agent> testedAgentPopulation = singleRound(mainWindow, currentPopulation);
        std::vector<Agent> selectedPopulation = rankSelection(testedAgentPopulation);
        std::cout << "Tested Population scores" << std::endl;
        printScoreBoard(testedAgentPopulation);
        std::cout << "Selected Population scores" << std::endl;
        printScoreBoard(selectedPopulation);
        currentPopulation = initializePopulation(i, selectedPopulation);
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

std::vector<Agent> Simulation::initializePopulation(unsigned genNum, std::vector<Agent> winners)
{
    std::vector<Agent> childrenAgents;
    for(Agent winner : winners){
        Agent a(genNum, winner);
        a.mutate();
        childrenAgents.push_back(a);
    }
    return childrenAgents;
}

/**
 * @brief Select an agent to pass on it's traits
 * @param agents
 */
std::vector<Agent> Simulation::propSelection(std::vector<Agent> &agents){
    // Compute total fitness
    std::sort(agents.begin(), agents.end());
    std::reverse(agents.begin(), agents.end());
    float totalFitness = 0;
    for(size_t i = 0; i < agents.size(); i++){
        Agent &a = agents[i];
        totalFitness += a.computeFitness();
    }

    std::vector<Agent> selectedAgents;
    for(size_t i = 0; i < agents.size(); i++){
        // random value between 0 and totalFitness
        float randVal = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/totalFitness));

        float selectionSum = 0;
        unsigned selectionIndex;
        for(size_t j = 0; j < agents.size(); j++){
            selectionSum += agents[j].computeFitness();
            if(selectionSum > randVal){
                selectionIndex = j;
                break;
            }
        }
        selectedAgents.push_back(agents[selectionIndex]);
    }
    return selectedAgents;
}

std::vector<Agent> Simulation::rankSelection(std::vector<Agent> &agents)
{
    std::sort(agents.begin(), agents.end());
    std::reverse(agents.begin(), agents.end());

    std::vector<float> rankProbVector(agents.size());
    float totalProb = 0;
    float p = simParams::rankProbability;
    for(size_t i = 0; i < rankProbVector.size(); i++){
        rankProbVector[i] = p * std::pow( (1-p) , i);
        totalProb += rankProbVector[i];
    }

    std::vector<Agent> selectedAgents;
    for(size_t i = 0; i < agents.size(); i++){
        // random value between 0 and 1
       float randVal = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/totalProb));
        float probSum = 0;
        unsigned selectionIndex;
        for(size_t j = 0; j < agents.size(); j++){
            probSum += rankProbVector[j];
            if(probSum > randVal){
                selectionIndex = j;
                break;
            }
        }
        selectedAgents.push_back(agents[selectionIndex]);
    }
    return selectedAgents;
}

void Simulation::printScoreBoard(std::vector<Agent> &agents)
{
    for(Agent &a : agents){
        std::cout << a.getId() << " : " << std::to_string(a.computeFitness()) << std::endl;
    }
}
