#include "game.hpp"
#include "sfmlvector.hpp"
#include "parameters.hpp"

Game::Game(sf::RenderWindow &window, std::unordered_map<std::__cxx11::string, Agent> &agents) :
      mWindow(window), allAgents(agents)
{
    float xSize = mWindow.getSize().x;
    float ySize = mWindow.getSize().y;
    sf::Vector2f screenMidpoint(xSize/2, ySize/2);
    float r = xSize/2 - xSize/10;

    // Circle used for spawning agents
    spawnCircle.setRadius(r);
    spawnCircle.setPointCount(allAgents.size());
    spawnCircle.setOrigin(r,r);
    spawnCircle.setPosition(screenMidpoint);

    // Initialize fair positioning of agents
    unsigned counter = 0;
    for(auto &kv : allAgents){
        Agent &a = kv.second;
        sf::Vector2f globalPointPosition = spawnCircle.getTransform().
                transformPoint(spawnCircle.getPoint(counter));

        a.setPosition(globalPointPosition);
        sf::Vector2f headingVector = screenMidpoint - globalPointPosition;
        float faceCenterHeading = SFMLVector::heading(headingVector);
        a.setRotation(faceCenterHeading);
        counter++;
    }
}
void Game::run(void){

    // For handling time
    sf::Clock clock;
    float totalElapsedTime = 0;

    // run the program as long as the window is open
    while (mWindow.isOpen())
    {
        float elapsedTime = clock.getElapsedTime().asSeconds();
        totalElapsedTime += elapsedTime;

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
        if(deadAgents.size() >= (allAgents.size() - 1)) break;
        // If time limit reached
        if(totalElapsedTime > simParams::maxTime) break;

    }

}
void Game::updatePhase(const float elapsedTime){
    // Agent Updates
    for(auto &kv : allAgents){
        Agent &thisAgent = kv.second;
        if(!thisAgent.hasDied()){
            thisAgent.fillInputVector(allAgents, bulletMap, mWindow);
            thisAgent.express(elapsedTime, bulletMap);
            thisAgent.update(elapsedTime, mWindow);
        }
        else deadAgents.insert(thisAgent.getId());
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
    for(auto &bulletId : bulletDeletionSet){
        bulletMap.erase(bulletId);
    }
    bulletDeletionSet.clear();
}

void Game::drawPhase(void){
    for(auto &kv : allAgents){
        Agent &thisAgent = kv.second;
        if(!thisAgent.hasDied()) mWindow.draw(thisAgent);
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
