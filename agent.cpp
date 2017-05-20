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
    idCount++; // Increment the ID for the next Agent

    isDead = false;
    canShoot = false;
    numHits = 0;
    health = agentParams::healthMax;
    stamina = agentParams::staminaMax;
    shotTimer = agentParams::shotChargeFrames;

    shape = sf::CircleShape(agentParams::agentRadius);
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(agentParams::agentRadius, agentParams::agentRadius);

    eye = sf::CircleShape(agentParams::eyeRadius);
    eye.setFillColor(sf::Color::Red);
    eye.setOrigin(agentParams::eyeRadius, agentParams::eyeRadius);
}

void Agent::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    target.draw(fov);
    target.draw(shape);
    target.draw(eye);
    target.draw(statusInd);

    UNUSED(states);
}

void Agent::update(const sf::RenderWindow &window){
    fov.update(shape.getRotation(), eye.getPosition());

    bool isOutOfBounds = EDGE::INSIDE != anyOutOfBounds(window, shape.getRadius());
    if(isOutOfBounds) shape.setPosition(lastPosition);
    // Either shot is charged or is recharging
    if(shotTimer >= agentParams::shotChargeFrames) {
        canShoot = true;
    }
    else{
        shotTimer += 1;
    }
    // Stamina regen
    if(stamina < agentParams::staminaMax) stamina += agentParams::staminaRegen;
    // Calculate new eye position
    sf::Vector2f heading = SFMLVector::vectorHeading(shape.getRotation());
    eye.setPosition(shape.getPosition() + heading*shape.getRadius());

    statusInd.update(shape.getPosition(), inputVector);
}

void Agent::express(std::unordered_map<std::string, Bullet>& bulletMap){
    lastPosition = shape.getPosition(); // Remember last position before movement occurs
    if(id=="Player") applyInputs(bulletMap);
    else{
        dna.brain.feedforward(inputVector);
        Eigen::VectorXf outputVector = dna.brain.computePrediction();
        bool thrustForward = outputVector(nnParam::thrustIndex) == nnParam::floatTrue;
        bool rotateRight = outputVector(nnParam::rotateRightIndex) == nnParam::floatTrue;
        bool rotateLeft = outputVector(nnParam::rotateLeftIndex) == nnParam::floatTrue;
        bool wantsToShoot = outputVector(nnParam::shootIndex) == nnParam::floatTrue;

        if(wantsToShoot) shoot(bulletMap);
        if(thrustForward) thrust();
        if(rotateRight && rotateLeft){} // Attempt to remove bias of turning a certain direction
        else if(rotateRight) rotate(1);
        else if(rotateLeft) rotate(-1);
    }
}

void Agent::applyInputs(std::unordered_map<std::string, Bullet>& bulletMap){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) thrust();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) rotate(1);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) rotate(-1);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) shoot(bulletMap);
}

void Agent::thrust(void){
    if(stamina > 0){
        sf::Vector2f thrustVector = SFMLVector::vectorHeading(shape.getRotation());
        sf::Vector2f addedVelocity = thrustVector*agentParams::thrustVelocity;
        sf::Vector2f newPosition = shape.getPosition() + addedVelocity;
        shape.setPosition(newPosition);
        stamina--;
    }

}

void Agent::rotate(const short direction){
    if(stamina > 0){
        shape.rotate(direction*agentParams::rotationVelocity);
        stamina--;
    }

}

void Agent::shoot(std::unordered_map<std::string, Bullet>& bulletMap){
    if(canShoot){
        Bullet b(eye.getPosition(), shape.getRotation(), *this);
        bulletMap.insert(std::make_pair(b.getId(), b));
        shotTimer = 0;
        canShoot = false;
    }
}

void Agent::fillInputVector(
        const std::unordered_map<std::string, Agent>& agentMap,
        std::unordered_map<std::string, Bullet>& bulletMap,
        const sf::RenderWindow &window){


    checkAgents(agentMap);
    checkBullets(bulletMap);

    inputVector(nnParam::canShootIndex) = static_cast<float>(canShoot);
    sf::Vector2f normalizedPosition = computeNormalizedPosition(
                shape.getPosition(),
                window.getSize().x,
                window.getSize().y
                );
    inputVector(nnParam::posXIndex) = normalizedPosition.x;
    inputVector(nnParam::posYIndex) = normalizedPosition.y;

    inputVector(nnParam::staminaIndex) = computeNormalizedStamina();
}

void Agent::checkBullets(std::unordered_map<std::string, Bullet> &bulletMap){
    // Bullet Field of vision checks and collision
    inputVector(nnParam::seeBulletIndex) = nnParam::floatFalse;
    for(auto &kvBullet : bulletMap){
        Bullet &b = kvBullet.second;
        // If it's not your own bullet
        if(id != b.getParentId()){
            if(canSeeEntity(b)) inputVector(nnParam::seeBulletIndex) = nnParam::floatTrue;

            bool isBulletCollision = SFMLVector::circToCircCollision(b.getShape(),shape);
            if(isBulletCollision){
                health--;
                if(health < 1) isDead = true;
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
        if(!thatAgent.hasDied() && thatAgent.getId() != id){
            if(hasAgentInSights(thatAgent)) inputVector(nnParam::sightsIndex) = nnParam::floatTrue;
            if(canSeeEntity(thatAgent)) inputVector(nnParam::seeAgentIndex) = nnParam::floatTrue;
        }
    }
}

bool Agent::canSeeEntity(const Entity &thatEntity) const{
    return fov.canSeeEntity(*this, thatEntity);
}

bool Agent::hasAgentInSights(const Agent &thatAgent) const{
    return fov.hasAgentInSights(thatAgent);
}

sf::Vector2f Agent::computeNormalizedPosition(const sf::Vector2f &pos, const float xMax, const float yMax) const{
    float normX = pos.x/xMax;
    float normY = pos.y/yMax;
    return sf::Vector2f(normX, normY);
}

float Agent::computeNormalizedStamina(void) const{
    return stamina/agentParams::staminaMax;
}

// Getters and setters
void Agent::setId(const std::string newId){
    id = newId;
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
    float fitness = 2*numHits + health;
    return fitness;
}
