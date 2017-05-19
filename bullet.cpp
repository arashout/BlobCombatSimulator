#include "bullet.hpp"
#include "sfmlvector.hpp"
#include "agent.hpp"
#include "parameters.hpp"

// Setup static variable that keeps track of number of bullets fired
// This is used for unique id creation
long Bullet::count = 0;

Bullet::Bullet(const sf::Vector2f p,
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
    velocity = bulletParams::baseSpeed*SFMLVector::vectorHeading(heading);
}
void Bullet::update(const sf::RenderWindow &window){
    sf::Vector2f newPosition = shape.getPosition() + velocity;
    shape.setPosition(newPosition);

    if(EDGE::INSIDE != outOfBounds(window, shape.getRadius())) expired = true;
}
bool Bullet::isExpired(void) const{
    return expired;
}
void Bullet::setExpiry(bool expiryValue){
    expired = expiryValue;
}
std::string Bullet::getParentId(void) const{
    return parentAgent.getId();
}
void Bullet::incrementHits(void){
    parentAgent.incrementHits();
}
