#include "agent.hpp"
#include "sfmlvector.hpp"
#include <math.h>
#include <iostream>
#include "parameters.hpp"

class Agent;

Agent::FieldOfVision::FieldOfVision(Agent &parentAgent) :
thisAgent(parentAgent)
{
    // Dumb way of initializing rays, but there's only 3 so no big deal
    ray1[0] = sf::Vertex(sf::Vector2f(0,0), sf::Color::Green);
    ray1[1] = sf::Vertex(sf::Vector2f(0,0), sf::Color::Green);
    ray2[0] = sf::Vertex(sf::Vector2f(0,0), sf::Color::Green);
    ray2[1] = sf::Vertex(sf::Vector2f(0,0), sf::Color::Green);
    raySights[0] = sf::Vertex(sf::Vector2f(0,0), sf::Color::Red);
    raySights[1] = sf::Vertex(sf::Vector2f(0,0), sf::Color::Red);
}
void Agent::FieldOfVision::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    target.draw(ray1, 2, sf::Lines);
    target.draw(ray2, 2, sf::Lines);
    target.draw(raySights, 2, sf::Lines);
    UNUSED(states);
}
void Agent::FieldOfVision::update(const float heading, const sf::Vector2f &position){
    updateRays(heading, position);
    // Update curHeadingVector for collision test methods
    // Note: This is already normalized!
    curHeadingVector = SFMLVector::vectorHeading(heading);
}

bool Agent::FieldOfVision::canSeeEntity(const Agent &thisAgent, const Entity &thatEntity) const{
    // Check that agent is within viewing distance
    sf::Vector2f agentToEntity = thatEntity.getPosition() - thisAgent.getPosition();
    if(SFMLVector::magnitude(agentToEntity) > fovParams::viewingDistance) return false;
    // TODO - Factor in radius into this calculation
    // Within viewing angle
    sf::Vector2f agentToEntityNorm = SFMLVector::normalize(agentToEntity);
    float angle = acos(SFMLVector::dot(curHeadingVector,agentToEntityNorm))*RAD2DEG;
    if(angle < fovParams::viewingAngle) return true;
    else return false;
}
bool Agent::FieldOfVision::hasAgentInSights(const Agent &thatAgent) const{
    sf::Vector2f lineSight[2] =  {raySights[0].position, raySights[1].position};
    if(SFMLVector::lineCircleCollision(lineSight, thatAgent.getShape())){
        return true;
    }
    else return false;
}
void Agent::FieldOfVision::updateRays(const float heading, const sf::Vector2f &position){
    // Update ray 1 position
    ray1[0].position = position;
    float rayAngle = (heading - fovParams::viewingAngle)*DEG2RAD;
    sf::Vector2f endPoint1(
                position.x + std::sin(rayAngle)*fovParams::viewingDistance,
                position.y - std::cos(rayAngle)*fovParams::viewingDistance
                );
    ray1[1].position = endPoint1;
    // Update ray 2 position
    ray2[0].position = position;
    rayAngle = (heading + fovParams::viewingAngle)*DEG2RAD;
    sf::Vector2f endPoint2 = sf::Vector2f(
                position.x + std::sin(rayAngle)*fovParams::viewingDistance,
                position.y - std::cos(rayAngle)*fovParams::viewingDistance
                );
    ray2[1].position = endPoint2;
    // Update sights position
    raySights[0].position = position;
    raySights[1] = curHeadingVector*fovParams::viewingDistance + position;
}
