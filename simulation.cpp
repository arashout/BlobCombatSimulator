#include "simulation.hpp"
#include "game.hpp"
#include "simulationparameters.hpp"
#include <iostream>
#include <algorithm>

bool agentCmp(const Agent &a1,const Agent &a2){
   return a1.computeFitness() < a2.computeFitness();
}

Simulation::Simulation() {
    sf::RenderWindow mainWindow(sf::VideoMode(600, 600), "Blob Combat Simulator!");
    // Initialize generation 0 population
    std::vector<Agent> startPopulation = initializePopulation(0);
    std::vector<Agent> testedAgentPopulation = singleRound(mainWindow, startPopulation);
    scoreAgents(testedAgentPopulation);

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

void Simulation::scoreAgents(std::vector<Agent> &agents){
    std::sort(agents.begin(), agents.end(), agentCmp);
    std::reverse(agents.begin(), agents.end());
    for(Agent &a : agents){
        std::cout << a.getId() << " : " << std::to_string(a.computeFitness()) << std::endl;
    }
}

std::vector<Agent> Simulation::initializePopulation(unsigned genNum){
    std::vector<Agent> agents;
    for(size_t i = 0; i < (simParams::numBatches * simParams::batchSize); i++){
        Agent a(genNum);
        agents.push_back(a);
    }
    return agents;
}
