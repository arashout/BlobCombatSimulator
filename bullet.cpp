#include "bullet.hpp"
#include "sfmlvector.hpp"

// Setup static variable that keeps track of number of bullets fired
// This is used for unique id creation
long Bullet::count = 0;

Bullet::Bullet(const sf::Vector2f p,
               const sf::Vector2f v,
               const float heading){
    // ID creation
    id = "B" + std::to_string(count);
    count++;

    characterLength = radius;
    // Shape setup
    shape = sf::CircleShape(radius);
    shape.setOrigin(radius, radius);
    shape.setPosition(p);
    velocity = v + baseSpeed * SFMLVector::vectorHeading(heading);
}
void Bullet::update(const float dt, const sf::RenderWindow &window){
    kinematics(dt);
    if(EDGE::INSIDE != outOfBounds(window, characterLength)) expired = true;
}
bool Bullet::isExpired(void){
    return expired;
}
void Bullet::kinematics(const float dt){
    sf::Vector2f newPosition = shape.getPosition() + velocity*dt;
    shape.setPosition(newPosition);
}
