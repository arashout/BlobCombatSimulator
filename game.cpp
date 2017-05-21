#include "game.hpp"
#include "sfmlvector.hpp"
#include "parameters.hpp"
#include <string>
#include <sstream>



#include <iostream>
Game::Game(sf::RenderWindow &window, std::unordered_map<std::string, Agent> &agents, sf::RenderWindow &secondWindow) :
      mWindow(window), mSecondWindow(secondWindow), allAgents(agents)
{
    float xSize = mWindow.getSize().x;
    float ySize = mWindow.getSize().y;
    sf::Vector2f screenMidpoint(xSize/2, ySize/2);
    float r = xSize/2 - xSize/10;

    setupTexts();

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
        float faceCenterHeading = SFMLVector::heading(headingVector) - 180;
        a.setRotation(faceCenterHeading);
        counter++;
    }
}
void Game::run(int &simSpeed){

    // For handling time
    unsigned totalFrames = 0;
    mWindow.setFramerateLimit(gameParams::normalFPS * simSpeed);
    mWindow.setKeyRepeatEnabled(false);

    sf::Clock clock;
    sf::Time time;

    std::string gameSpeedString;
    std::string fpsString;

    // run the program as long as the window is open
    while (mWindow.isOpen())
    {
        time = clock.getElapsedTime();
        clock.restart().asSeconds();
        totalFrames++;
        handleEvents(simSpeed);

        // clear the window with black color
        mWindow.clear(sf::Color::Black);

        updatePhase();

        fpsString = "FPS: " + std::to_string(1.0f/time.asSeconds());
        fpsText.setString(fpsString);
        gameSpeedString = "Gamespeed " + std::to_string(simSpeed);
        gameSpeedText.setString(gameSpeedString);

        deletionPhase();

        drawPhase();
        drawInfoWindow();

        // end the current frame
        mWindow.display();


        // Last agent remaining marks end of game
        if(deadAgents.size() >= (allAgents.size() - 1)) break;
        // If time limit reached
        if(totalFrames > gameParams::framesPerGame) break;

    }
}

void Game::setupTexts()
{
    extern sf::Font globalFont;

    fpsText.setFont(globalFont);
    fpsText.setFont(globalFont);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setCharacterSize(25);
    fpsText.setPosition(0, 30);

    gameSpeedText.setFont(globalFont);
    gameSpeedText.setFillColor(sf::Color::White);
    gameSpeedText.setCharacterSize(25);
    gameSpeedText.setPosition(0, 0);
}
void Game::updatePhase(){
    // Agent Updates
    for(auto &kv : allAgents){
        Agent &thisAgent = kv.second;
        if(!thisAgent.hasDied()){
            thisAgent.setInputs(allAgents, bulletMap, mWindow);
            thisAgent.express(bulletMap);
            thisAgent.update(mWindow);
        }
        else deadAgents.insert(thisAgent.getId());
    }

    // Bullet updates - With range-for loop
    for(auto &kv : bulletMap)
    {
        Bullet &curBullet = kv.second;
        curBullet.update(mWindow);
        if(curBullet.isExpired()) bulletDeletionSet.insert(curBullet.getId());
    }
}
void Game::deletionPhase(void){
    for(auto &bulletId : bulletDeletionSet){
        bulletMap.erase(bulletId);
    }
    bulletDeletionSet.clear();
}

void Game::drawInfoWindow(void)
{
    mSecondWindow.clear(sf::Color::Black);
    mSecondWindow.draw(fpsText);
    mSecondWindow.draw(gameSpeedText);
    mSecondWindow.display();
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

void Game::handleEvents(int &simSpeed){
    sf::Event event;
    // check all the window's events that were triggered since the last iteration of the loop
    if (mWindow.pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed) mWindow.close();
        if (event.type == sf::Event::MouseButtonPressed){
            if(event.mouseButton.button == sf::Mouse::Left) simSpeed++;
            else if(event.mouseButton.button == sf::Mouse::Right) simSpeed--;

            if(simSpeed < 1) simSpeed = 1;
            mWindow.setFramerateLimit(gameParams::normalFPS * simSpeed);
        }
    }
}
