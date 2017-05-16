#include "simulation.hpp"
#include "game.hpp"
#include "simulationparameters.hpp"
#include <iostream>

Simulation::Simulation() {
    // Initialize generation 0 population
    initializePopulation(0);

    sf::RenderWindow mainWindow(sf::VideoMode(600, 600), "Blob Combat Simulator!");

    singleRound(mainWindow);
    scoreRound();

}

void Simulation::singleRound(sf::RenderWindow &window){
    std::unordered_map<std::string, Agent> currentBatch;

    unsigned counter = 1;

    for(auto &a : agentPopulation){
        currentBatch.insert(std::make_pair(a.getId(), a));

        if(counter % simParams::batchSize == 0){
            singleGame(currentBatch, window);
            currentBatch.clear();
        }
        counter++;
    }
}

void Simulation::singleGame(std::unordered_map<std::string, Agent> &batchAgents,
                            sf::RenderWindow &window){
    Game game(window, batchAgents);
    game.run();
}

void Simulation::scoreRound(void){

    for(Agent &a : agentPopulation){
        std::cout << a.getId() << " : " << std::to_string(a.computeFitness()) << std::endl;
    }
}

void Simulation::initializePopulation(unsigned genNum){
    agentPopulation.clear();
    for(size_t i = 0; i < (simParams::numBatches * simParams::batchSize); i++){
        Agent a(genNum);
        agentPopulation.push_back(a);
    }
}
