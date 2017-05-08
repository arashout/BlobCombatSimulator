#ifndef BULLET_HPP
#define BULLET_HPP

#include "entity.hpp"


class Bullet : public Entity
{
public:
    Bullet(const sf::Vector2f p,
           const sf::Vector2f v,
           const float heading, Entity &agent);
    void update(const float dt,const sf::RenderWindow &window);
    bool isExpired(void) const;
    std::string getParentId(void) const;

private:
    static long count;
    void kinematics(const float dt);
    bool expired = false;
    const float baseSpeed = 250;
    const float radius = 3;
    // Agent that fired this bullet
    Entity &agentParent;

};

#endif // BULLET_HPP
