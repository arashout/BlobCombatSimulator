#ifndef GAME_HPP
#define GAME_HPP

#include "SFML/Graphics.hpp"
#include "agent.hpp"
#include <unordered_map>
#include <unordered_set>
#include <string>

class Game
{
public:
    Game(sf::RenderWindow &window,std::unordered_map<std::string, Agent> &agents, sf::RenderWindow &secondWindow);
    void run(int &simSpeed);
private:
    sf::RenderWindow &mWindow;
    sf::RenderWindow &mSecondWindow;
    std::unordered_map<std::string, Bullet> bulletMap;
    std::unordered_set<std::string> bulletDeletionSet;
    std::unordered_map<std::string, Agent> &allAgents;
    std::unordered_set<std::string> deadAgents;

    void setupTexts(void);
    void handleEvents(int &simSpeed);
    void updatePhase();
    void drawPhase(void);
    void deletionPhase(void);
    void drawInfoWindow(void);

    void reset();
    void pause();

    sf::CircleShape spawnCircle;

    //Information Window
    sf::Text fpsText;
    sf::Text gameSpeedText;
};

#endif // GAME_HPP
