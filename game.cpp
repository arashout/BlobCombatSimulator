#include "game.hpp"
#include "sfmlvector.hpp"

Game::Game(sf::RenderWindow &window) :
    mWindow(window)
{
    Agent a1(15, static_cast<sf::Vector2f> (mWindow.getSize()) / 2.0f);
    a1.setId("Player");
    Agent a2(15, static_cast<sf::Vector2f> (mWindow.getSize()) / 3.0f);
    activeAgentMap.insert(std::make_pair(a1.getId(), a1));
    activeAgentMap.insert(std::make_pair(a2.getId(), a2));

}
void Game::run(void){

    // For handling time
    sf::Clock clock;


    // run the program as long as the window is open
    while (mWindow.isOpen())
    {
        float elapsedTime = clock.getElapsedTime().asSeconds();
        clock.restart();

        handleEvents();

        // clear the window with black color
        mWindow.clear(sf::Color::Black);

        updatePhase(elapsedTime);
        deletionPhase();
        drawPhase();

        // end the current frame
        mWindow.display();

        // Last agent remaining marks end of game
        if(activeAgentMap.size() == 1) break;
    }
}
void Game::updatePhase(const float elapsedTime){
    // Agent Updates
    for(auto &kv : activeAgentMap){
        Agent &thisAgent = kv.second;
        if(!thisAgent.hasDied()){
            thisAgent.fillInputVector(activeAgentMap, bulletMap, mWindow);
            thisAgent.execute(elapsedTime, bulletMap);
            thisAgent.update(elapsedTime, mWindow);
        }
        else deadAgentIds.insert(thisAgent.getId());
    }

    // Bullet updates - With range-for loop
    for(auto &kv : bulletMap)
    {
        Bullet &curBullet = kv.second;
        curBullet.update(elapsedTime, mWindow);
        if(curBullet.isExpired()) bulletDeletionSet.insert(curBullet.getId());
    }
}
void Game::deletionPhase(void){
    for(auto &agentId : deadAgentIds){
        // Add dead agent to deadAgent map
        Agent &agent = activeAgentMap.at(agentId);
        deadAgentMap.insert(std::make_pair(agent.getId(), agent));
        activeAgentMap.erase(agentId);
    }
    deadAgentIds.clear();

    for(auto &bulletId : bulletDeletionSet){
        bulletMap.erase(bulletId);
    }
    bulletDeletionSet.clear();
}

void Game::drawPhase(void){
    for(auto &kv : activeAgentMap){
        Agent &thisAgent = kv.second;
        mWindow.draw(thisAgent);
    }
    for(auto &kv : bulletMap){
        Bullet &curBullet = kv.second;
        mWindow.draw(curBullet);
    }
}

void Game::handleEvents(void){
    sf::Event event;
    // check all the window's events that were triggered since the last iteration of the loop
    while (mWindow.pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            mWindow.close();
    }
}
