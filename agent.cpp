#include "agent.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "sfmlvector.hpp"
#include <math.h>
#include <iostream>
#include "nnparameters.hpp"

// This is used for unique id creation
long Agent::idCount = 0;

Agent::Agent(unsigned genNum) : fov(*this), inputVector(NUM_INPUTS)
{
    id = "G" + std::to_string(genNum) + "A" + std::to_string(idCount);
    idCount++;
    // Defaults
    canShoot = true;
    isDead = false;
    timeAlive = 0.0;
    kills = 0;
    fitness = 0.0;
    shotTimer = shotChargeTime;

    // Ship shape and colour adjusted here
    shape = sf::CircleShape(agentRadius);
    shape.setFillColor(sf::Color::White);

    eye = sf::CircleShape(eyeRadius);
    eye.setFillColor(sf::Color::Red);

    // Convenienve
    shape.setOrigin(agentRadius, agentRadius);
    eye.setOrigin(eyeRadius, eyeRadius);

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
    kinematics(dt);
    fov.update(shape.getRotation(), eye.getPosition());
    // Mirror edges - Like in PACMAN
    resetPosition(outOfBounds(window, shape.getRadius()), window, shape.getRadius());
    // Recharge shot
    if(shotTimer >= shotChargeTime) {
        canShoot = true;
        shotTimer = 0;
    }
    else if(shotTimer < shotChargeTime){
        shotTimer += dt;
        canShoot = false;
    }

    timeAlive += dt;
}

void Agent::execute(const float dt, std::unordered_map<std::__cxx11::string, Bullet> &bulletMap){
    if(id=="Player") applyInputs(dt, bulletMap);
    else express(dt, bulletMap);
}

void Agent::kinematics(const float dt){
    sf::Vector2f newPosition = shape.getPosition() + velocity*dt;
    shape.setPosition(newPosition);
    // Calculate eye position
    sf::Vector2f heading = SFMLVector::vectorHeading(shape.getRotation());
    eye.setPosition(shape.getPosition() + heading*shape.getRadius());
}

void Agent::applyInputs(const float dt, std::unordered_map<std::string, Bullet>& bulletMap){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) thrust(dt, 1);
    else velocity -= velocity*velocityDecay*dt;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) rotate(dt, 1);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) rotate(dt, -1);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) shoot(bulletMap, true);
}

void Agent::express(const float dt, std::unordered_map<std::string, Bullet>& bulletMap){
    dna.brain.feedforward(inputVector);
    Eigen::VectorXf outputVector = dna.brain.computePrediction();
    shoot(bulletMap, outputVector(nnParam::shootIndex));
    thrust(dt, outputVector(nnParam::thrustIndex) );

    bool rotateRight = outputVector(nnParam::rotateRightIndex) == 1.0f;
    bool rotateLeft = outputVector(nnParam::rotateLeftIndex == 1.0f);
    if(rotateRight && rotateLeft){} // Attempt to remove bias of turning a certain direction
    else if(rotateRight) rotate(dt, 1);
    else if(rotateLeft) rotate(dt, -1);
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

void Agent::shoot(std::unordered_map<std::string, Bullet>& bulletMap,const bool wantsToShoot){
    if(wantsToShoot && canShoot){
        Bullet b(eye.getPosition() ,velocity, shape.getRotation(), *this);
        bulletMap.insert(std::make_pair(b.getId(), b));
    }
}

void Agent::setId(const std::string newId){
    id = newId;
}

bool Agent::canSeeEntity(const Entity &thatEntity) const{
    return fov.canSeeEntity(*this, thatEntity);
}

bool Agent::hasAgentInSights(const Agent &thatAgent) const{
    return fov.hasAgentInSights(thatAgent);
}

void Agent::fillInputVector(
        const std::unordered_map<std::string, Agent>& agentMap,
        std::unordered_map<std::string, Bullet>& bulletMap,
        const sf::RenderWindow &window){


    checkAgents(agentMap);
    checkBullets(bulletMap);

    inputVector(nnParam::shotTimerIndex) = computeNormalizedShotTimer();
    sf::Vector2f normalizedPosition = computeNormalizedPosition(
                shape.getPosition(),
                window.getSize().x,
                window.getSize().y
                );
    inputVector(nnParam::posXIndex) = normalizedPosition.x;
    inputVector(nnParam::posYIndex) = normalizedPosition.y;
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
                b.incrementParentKills();
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
        if(!thatAgent.hasDied() && thatAgent.getId() != id){
            if(hasAgentInSights(thatAgent)) inputVector(nnParam::seeAgentIndex) = nnParam::floatTrue;

            if(canSeeEntity(thatAgent)) inputVector(nnParam::seeAgentIndex) = nnParam::floatTrue;
        }
    }
}

float Agent::computeNormalizedShotTimer(void) const{
    return shotTimer/shotChargeTime;
}

sf::Vector2f Agent::computeNormalizedPosition(const sf::Vector2f &pos, const float xMax, const float yMax) const{
    float normX = pos.x/xMax;
    float normY = pos.y/yMax;
    return sf::Vector2f(normX, normY);
}

bool Agent::hasDied(void) const{
    return isDead;
}

void Agent::setPosition(const sf::Vector2f p){
    shape.setPosition(p);
}

void Agent::setRotation(const float heading){
    shape.setRotation(heading);
}

void Agent::incrementKillCount(void){
    kills++;
}

float Agent::computeFitness(void){
    if(fitness != 0.0) return fitness; // Cached

    if(isDead) fitness = kills*10 + timeAlive*3;
    else fitness = kills*10 + (1/timeAlive)*10 + 20;
    return fitness;
}
