#include "bullet.hpp"
#include "sfmlvector.hpp"
#include "agent.hpp"
#include "parameters.hpp"

// Setup static variable that keeps track of number of bullets fired
// This is used for unique id creation
long Bullet::count = 0;

Bullet::Bullet(const sf::Vector2f p,
               const sf::Vector2f v,
               const float heading,
               Agent &agent) :
    parentAgent(agent)
{
    // ID creation
    id = "B" + std::to_string(count);
    count++;

    // Shape setup
    shape = sf::CircleShape(bulletParams::radius);
    shape.setOrigin(bulletParams::radius, bulletParams::radius);
    shape.setPosition(p);
    velocity = v + bulletParams::baseSpeed*SFMLVector::vectorHeading(heading);
}
void Bullet::update(const float dt, const sf::RenderWindow &window){
    kinematics(dt);
    if(EDGE::INSIDE != outOfBounds(window, shape.getRadius())) expired = true;
}
bool Bullet::isExpired(void) const{
    return expired;
}
void Bullet::setExpiry(bool expiryValue){
    expired = expiryValue;
}

void Bullet::kinematics(const float dt){
    sf::Vector2f newPosition = shape.getPosition() + velocity*dt;
    shape.setPosition(newPosition);
}
std::string Bullet::getParentId(void) const{
    return parentAgent.getId();
}
void Bullet::incrementParentKills(void){
    parentAgent.incrementKillCount();
}
