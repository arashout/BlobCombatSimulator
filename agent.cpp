#include "agent.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "sfmlvector.hpp"
#include <math.h>
#include <iostream>
#include "nnparameters.hpp"

// This is used for unique id creation
long Agent::idCount = 0;

Agent::Agent(const float agentRadius,const sf::Vector2f position) :
fov(*this), inputVector(NUM_INPUTS)
{
    id = "A" + std::to_string(idCount);
    idCount++;
    // Defaults
    canShoot = true;
    intendToShoot = false;
    isDead = false;
    timeAlive = 0.0;
    kills = 0;
    score = 0;
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

    UNUSED(states);
}

void Agent::update(const float dt, const sf::RenderWindow &window){
    if(id == "Player") applyInputs(dt);
    kinematics(dt);
    fov.update(shape.getRotation(), eye.getPosition());
    // Mirror edges - Like in PACMAN
    resetPosition(outOfBounds(window, shape.getRadius()), window, shape.getRadius());
    // Recharge shot
    if(shotTimer >= shotChargeTime) canShoot = true;
    else if(shotTimer < shotChargeTime){
        shotTimer += dt;
    }

    timeAlive += dt;
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

void Agent::shoot(std::unordered_map<std::string, Bullet>& bulletMap){
    Agent &thisAgent = *this;
    if(intendToShoot == true){
        Bullet b(eye.getPosition() ,velocity, shape.getRotation(), thisAgent);
        bulletMap.insert(std::make_pair(b.getId(), b));
        intendToShoot = false;
    }
}

void Agent::setId(const std::string newId){
    id = newId;
}

bool Agent::canSeeEntity(const Entity &thatEntity) const{
    return fov.canSeeEntity(*this, thatEntity);
}

bool Agent::hasAgentInSights(const Agent &thatAgent) const{
    return fov.hasAgentInSights(*this, thatAgent);
}

void Agent::fillInputVector(
        const std::unordered_map<std::string, Agent>& agentMap,
        std::unordered_map<std::string, Bullet>& bulletMap,
        const sf::RenderWindow &window){

    checkAgents(agentMap);
    checkBullets(bulletMap);
}

void Agent::checkBullets(std::unordered_map<std::string, Bullet> &bulletMap){
    // Bullet Field of vision checks and collision
    inputVector(nnParam::seeBulletIndex) = nnParam::floatFalse;
    for(auto &kvBullet : bulletMap){
        Bullet &b = kvBullet.second;
        // If it's not your own bullet
        if(id != b.getParentId()){
            if(canSeeEntity(b)) inputVector(nnParam::seeBulletIndex) = nnParam::seeBulletIndex;

            bool isBulletCollision = SFMLVector::circToCircCollision(
                        b.getShape(),
                        shape
                        );
            if(isBulletCollision){
                isDead = true;
                b.setExpiry(true);
            }
        }
    }
}

void Agent::checkAgents(const std::unordered_map<std::string, Agent> &agentMap){
    // Agent Field of vision checks!
    inputVector(nnParam::sightsIndex) = nnParam::floatFalse;
    inputVector(nnParam::seeAgentIndex) = nnParam::floatFalse;

    for(auto &kv2 : agentMap){
        const Agent &thatAgent = kv2.second;

        if(hasAgentInSights(thatAgent)) inputVector(nnParam::seeAgentIndex) = nnParam::floatTrue;

        if(canSeeEntity(thatAgent)) inputVector(nnParam::seeAgentIndex) = nnParam::floatTrue;
    }
}

bool Agent::hasDied(void){
    return isDead;
}
