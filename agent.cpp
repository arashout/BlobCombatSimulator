#include "agent.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "sfmlvector.hpp"
#include <math.h>
#include <iostream>
#include "parameters.hpp"

// This is used for unique id creation
long Agent::idCount = 0;

Agent::Agent(unsigned genNum) : inputVector(nnParam::numInputs)
{
    setup(genNum);
}

Agent::Agent(unsigned genNum, Agent parent) :
    dna(parent.dna), inputVector(nnParam::numInputs)
{
    setup(genNum);
}

void Agent::setup(unsigned genNum)
{
    id = "G" + std::to_string(genNum) + "A" + std::to_string(idCount);
    idCount++;
    // Defaults
    canShoot = true;
    isDead = false;
    timeAlive = 0.0;
    numHits = 0;
    numHitten = 0;
    shotTimer = agentParams::shotChargeTime;

    // Ship shape and colour adjusted here
    const float &r = agentParams::agentRadius;
    shape = sf::CircleShape(r);
    shape.setFillColor(sf::Color::White);

    eye = sf::CircleShape(agentParams::eyeRadius);
    eye.setFillColor(sf::Color::Red);

    // Convenienve
    shape.setOrigin(r, r);
    eye.setOrigin(agentParams::eyeRadius, agentParams::eyeRadius);

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
    if(shotTimer >= agentParams::shotChargeTime) {
        canShoot = true;
        shotTimer = 0;
    }
    else if(shotTimer < agentParams::shotChargeTime){
        shotTimer += dt;
        canShoot = false;
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
    velocity += thrustVector * agentParams::thrustPower * dt * direction;
    // Limit velocity
    if(SFMLVector::magnitude(velocity) > agentParams::terminalSpeed){
        velocity = SFMLVector::limit(velocity, agentParams::terminalSpeed);
    }
}

void Agent::rotate(const float dt, const float direction){
    shape.rotate(dt*direction*agentParams::rotatePower);
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
//    sf::Vector2f normalizedPosition = computeNormalizedPosition(
//                shape.getPosition(),
//                window.getSize().x,
//                window.getSize().y
//                );
//    inputVector(nnParam::posXIndex) = normalizedPosition.x;
//    inputVector(nnParam::posYIndex) = normalizedPosition.y;
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
                numHitten++;
                if(numHitten > agentParams::numLives) isDead = true;
                b.incrementHits();
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
        if( thatAgent.getId() != id){
            if(hasAgentInSights(thatAgent)) inputVector(nnParam::seeAgentIndex) = nnParam::floatTrue;

            if(canSeeEntity(thatAgent)) inputVector(nnParam::seeAgentIndex) = nnParam::floatTrue;
        }
    }
}

float Agent::computeNormalizedShotTimer(void) const{
    return shotTimer/agentParams::shotChargeTime;
}

sf::Vector2f Agent::computeNormalizedPosition(const sf::Vector2f &pos, const float xMax, const float yMax) const{
    float normX = pos.x/xMax;
    float normY = pos.y/yMax;
    return sf::Vector2f(normX, normY);
}

void Agent::setPosition(const sf::Vector2f p){
    shape.setPosition(p);
}

void Agent::setRotation(const float heading){
    shape.setRotation(heading);
}

bool Agent::hasDied() const
{
    return isDead;
}

void Agent::incrementHits(void){
    numHits++;
}

float Agent::computeFitness(void) const{
    float fitness = numHits*4 + !isDead*2 + timeAlive - numHitten*2;
    if(fitness < 0) return 0;
    return fitness;
}
