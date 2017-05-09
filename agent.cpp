#include "agent.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "sfmlvector.hpp"
#include <math.h>
#include <iostream>

// This is used for unique id creation
long Agent::count = 0;

Agent::Agent(const float agentRadius,const sf::Vector2f position) :
fov(*this) // Pass a reference of the parent to field of view
{
    id = "Agent" + std::to_string(count);
    count++;
    // Shooting defaults
    canShoot = true;
    intendToShoot = false;
    shotTimer = shotChargeTime;

    // Ship shape and colour adjusted here
    shape = sf::CircleShape(agentRadius);
    shape.setFillColor(sf::Color::White);

    eye = sf::CircleShape(eyeRadius);
    eye.setFillColor(sf::Color::Red);

    // Convenienve
    shape.setOrigin(agentRadius, agentRadius);
    eye.setOrigin(eyeRadius, eyeRadius);

    shape.setPosition(position);
    velocity = sf::Vector2f(0,0);

}
void Agent::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    target.draw(fov);
    target.draw(shape);
    // Draw eye
    target.draw(eye);
}
void Agent::update(const float dt, const sf::RenderWindow &window){
    if(id == "Player") applyInputs(dt);
    kinematics(dt);
    fov.update(shape.getRotation(), eye.getPosition());
    // Mirror edges - Like in PACMAN
    resetPosition(outOfBounds(window, shape.getRadius()),
                  window, shape.getRadius());
    // Recharge shot
    if(shotTimer >= shotChargeTime) canShoot = true;
    else if(shotTimer < shotChargeTime){
        shotTimer += dt;
    }

}

void Agent::kinematics(const float dt){
    sf::Vector2f newPosition = shape.getPosition() + velocity*dt;
    shape.setPosition(newPosition);
    // Calculate eye position
    sf::Vector2f heading = SFMLVector::vectorHeading(shape.getRotation());
    eye.setPosition(shape.getPosition() + heading*shape.getRadius());
}

void Agent::applyInputs(const float dt){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) thrust(dt, 1);
    else velocity -= velocity*velocityDecay*dt;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) rotate(dt, 1);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) rotate(dt, -1);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canShoot){
        intendToShoot = true;
        shotTimer = 0;
        canShoot = false;
    }
}

void Agent::thrust(const float dt, const float direction){
    sf::Vector2f thrustVector = SFMLVector::vectorHeading(shape.getRotation());
    velocity += thrustVector * thrustPower * dt * direction;
    // Limit velocity
    if(SFMLVector::magnitude(velocity) > terminalSpeed){
        velocity = SFMLVector::limit(velocity, terminalSpeed);
    }
}

void Agent::rotate(const float dt, const float direction){
    shape.rotate(dt*direction*rotatePower);
}

void Agent::shoot(std::unordered_map<std::string, Bullet>& bulletTable){

    if(intendToShoot == true){
        Bullet b(eye.getPosition() ,velocity, shape.getRotation(), *this);
        bulletTable.insert(std::make_pair(b.getId(), b));
        intendToShoot = false;
    }
}
void Agent::setId(const std::string newId){
    id = newId;
}
bool Agent::canSeeEntity(const Agent &thisAgent,const Entity &thatEntity) const{
    return fov.canSeeEntity(thisAgent, thatEntity);
}
bool Agent::hasAgentInSights(const Agent &thisAgent, const Agent &thatAgent) const{
    return fov.hasAgentInSights(thisAgent, thatAgent);
}
