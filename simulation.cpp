#include "simulation.hpp"
#include "game.hpp"
#include "simulationparameters.hpp"

Simulation::Simulation()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(600, 600), "Blob Combat Simulator!");

    std::unordered_map<std::string, Agent> agentPopulation = initializePopulation(0);



}

void Simulation::singleRound(std::unordered_map<std::string, Agent> &batchAgents){
}

void Simulation::singleGame(std::unordered_map<std::string, Agent> &batchAgents){
    Game game(window);
    game.run();
}


std::unordered_map<std::string, Agent> Simulation::initializePopulation(unsigned genNum){
    std::unordered_map<std::string, Agent> agentPop;

    for(size_t i = 0; i < (simParams::numBatches * simParams::batchSize); i++){
        Agent.resetIdCount();
        Agent a(genNum);
        agentPop.insert(std::make_pair(a.getId(), a));
    }

    return agentPop;
}
