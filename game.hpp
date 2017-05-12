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
    Game(sf::RenderWindow &window);
    void run(void);
private:
    sf::RenderWindow &mWindow;
    std::unordered_map<std::string, Bullet> bulletMap;
    std::unordered_set<std::string> bulletDeletionSet;
    std::unordered_map<std::string, Agent> agentMap;

    void handleEvents(void);
    void updatePhase(const float elapsedTime);
    void drawPhase(void);
    void deletionPhase(void);

    void reset();
    void pause();
};

#endif // GAME_HPP
