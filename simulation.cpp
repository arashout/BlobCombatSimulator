#include "simulation.hpp"
#include "game.hpp"
#include "simulationparameters.hpp"

Simulation::Simulation() {
    std::unordered_map<std::string, Agent> agentPopulation = initializePopulation(0);

    sf::RenderWindow mainWindow(sf::VideoMode(600, 600), "Blob Combat Simulator!");
    singleRound(agentPopulation, mainWindow);

}

void Simulation::singleRound(std::unordered_map<std::string, Agent> &currentPopulation,
                             sf::RenderWindow &window){
    std::unordered_map<std::string, Agent> currentBatch;

    unsigned counter = 1;

    for(auto &kv : currentPopulation){
        Agent &a = kv.second;
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


std::unordered_map<std::string, Agent> Simulation::initializePopulation(unsigned genNum){
    std::unordered_map<std::string, Agent> agentPop;

    for(size_t i = 0; i < (simParams::numBatches * simParams::batchSize); i++){
        Agent a(genNum);
        agentPop.insert(std::make_pair(a.getId(), a));
    }

    return agentPop;
}
