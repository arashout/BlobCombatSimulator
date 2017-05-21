#include "agent.hpp"
#include "sfmlvector.hpp"
#include <math.h>
#include <iostream>
#include "parameters.hpp"
#include "fieldofvision.hpp"

class Agent;

FieldOfVision::FieldOfVision(){
    // Dumb way of initializing rays, but there's only 3 so no big deal
    ray1[0] = sf::Vertex(sf::Vector2f(0,0), sf::Color::Green);
    ray1[1] = sf::Vertex(sf::Vector2f(0,0), sf::Color::Green);
    ray2[0] = sf::Vertex(sf::Vector2f(0,0), sf::Color::Green);
    ray2[1] = sf::Vertex(sf::Vector2f(0,0), sf::Color::Green);

    viewAngle = fovParams::maxViewAngle;
}
void FieldOfVision::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    target.draw(ray1, 2, sf::Lines);
    target.draw(ray2, 2, sf::Lines);
    UNUSED(states);
}
void FieldOfVision::update(const float heading, const sf::Vector2f &position){
    updateRays(heading, position);
    // Update curHeadingVector for collision test methods
    // Note: This is already normalized!
    curHeadingVector = SFMLVector::vectorHeading(heading);
}

bool FieldOfVision::canSeeEntity(const Agent &thisAgent,const Entity &thatEntity) const{
    // Check that agent is within viewing distance
    sf::Vector2f agentToEntity = thatEntity.getPosition() - thisAgent.getPosition();
    if(SFMLVector::magnitude(agentToEntity) > fovParams::viewingDistance) return false;
    // TODO - Factor in radius into this calculation
    // Within viewing angle
    sf::Vector2f agentToEntityNorm = SFMLVector::normalize(agentToEntity);
    float angle = acos(SFMLVector::dot(curHeadingVector,agentToEntityNorm))*RAD2DEG;
    if(angle < viewAngle) return true;
    else return false;
}

float FieldOfVision::getViewAngle(void) const
{
    return viewAngle;
}

void FieldOfVision::setViewAngle(const float newViewAngle)
{
    if(newViewAngle < 0 || newViewAngle > fovParams::maxViewAngle) throw "Error";
    viewAngle = newViewAngle;
}

void FieldOfVision::updateRays(const float heading, const sf::Vector2f &position){
    // Update ray 1 position
    ray1[0].position = position;
    float rayAngle1 = (heading - viewAngle)*DEG2RAD;
    sf::Vector2f endPoint1(
                position.x + std::sin(rayAngle1)*fovParams::viewingDistance,
                position.y - std::cos(rayAngle1)*fovParams::viewingDistance
                );
    ray1[1].position = endPoint1;

    // Update ray 2 position
    ray2[0].position = position;
    float rayAngle2 = (heading + viewAngle)*DEG2RAD;
    sf::Vector2f endPoint2 = sf::Vector2f(
                position.x + std::sin(rayAngle2)*fovParams::viewingDistance,
                position.y - std::cos(rayAngle2)*fovParams::viewingDistance
                );
    ray2[1].position = endPoint2;
}
