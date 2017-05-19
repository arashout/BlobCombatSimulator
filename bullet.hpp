#ifndef BULLET_HPP
#define BULLET_HPP

#include "entity.hpp"
class Agent;

class Bullet : public Entity
{
public:
    Bullet(
           const sf::Vector2f p,
           const float heading,
           Agent &agent
    );
    void update(const sf::RenderWindow &window);
    bool isExpired(void) const;
    void setExpiry(bool expiryValue);
    std::string getParentId(void) const;
    void incrementHits(void);

private:
    static long count;
    bool expired = false;
    sf::Vector2f velocity;
    // Agent that fired this bullet
    Agent &parentAgent;

};

#endif // BULLET_HPP
