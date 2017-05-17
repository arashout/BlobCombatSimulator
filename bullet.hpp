#ifndef BULLET_HPP
#define BULLET_HPP

#include "entity.hpp"
class Agent;

class Bullet : public Entity
{
public:
    Bullet(
           const sf::Vector2f p,
           const sf::Vector2f v,
           const float heading,
           Agent &agent
    );
    void update(const float dt,const sf::RenderWindow &window);
    bool isExpired(void) const;
    void setExpiry(bool expiryValue);
    std::string getParentId(void) const;
    void incrementParentKills(void);

private:
    static long count;
    void kinematics(const float dt);
    bool expired = false;
    // Agent that fired this bullet
    Agent &parentAgent;

};

#endif // BULLET_HPP
