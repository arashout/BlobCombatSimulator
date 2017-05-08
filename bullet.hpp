#ifndef BULLET_HPP
#define BULLET_HPP

#include "entity.hpp"

class Bullet : public Entity
{
public:
    Bullet(const sf::Vector2f p,
           const sf::Vector2f v,
           const float heading);
    void update(const float dt,const sf::RenderWindow &window);
    bool isExpired(void);
private:
    static long count;
    void kinematics(const float dt);
    bool expired = false;
    const float baseSpeed = 250;
    const float radius = 3;
};

#endif // BULLET_HPP
