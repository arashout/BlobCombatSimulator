#include "agent.hpp"
#include "sfmlvector.hpp"
#include <math.h>

Agent::FieldOfVision::FieldOfVision(){
    // Dumb way of initializing rays, but there's only 3 so no big deal
    ray1[0] = sf::Vertex(sf::Vector2f(0,0), sf::Color::Green);
    ray1[1] = sf::Vertex(sf::Vector2f(0,0), sf::Color::Green);
    ray2[0] = sf::Vertex(sf::Vector2f(0,0), sf::Color::Green);
    ray2[1] = sf::Vertex(sf::Vector2f(0,0), sf::Color::Green);
}
void Agent::FieldOfVision::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    target.draw(ray1, 2, sf::Lines);
    target.draw(ray2, 2, sf::Lines);
}
void Agent::FieldOfVision::update(const float heading, const sf::Vector2f &position){
    updateRays(heading, position);
    // Update curHeadingVector for collision test methods
    curHeadingVector = SFMLVector::vectorHeading(heading);
}

bool Agent::FieldOfVision::canSeeAgent(const Agent thisAgent,const Agent thatAgent){
    // Check that agent is within viewing distance
    sf::Vector2f agentToAgent = thisAgent.getPosition() - thatAgent.getPosition();
    float magVal= SFMLVector::magnitude(agentToAgent);
    if(magVal > viewingDistance) return false;
    // TODO - Factor in radius into this calculation
    // Within viewing angle
    sf::Vector2f agentFacingNorm = SFMLVector::normalize(curHeadingVector);
    sf::Vector2f agentToAgentNorm = SFMLVector::normalize(agentToAgent);
    float angle = acos(SFMLVector::dot(agentFacingNorm,agentToAgentNorm))*RAD2DEG;
    // TODO - Explain this more
    angle = (angle - 180)*-1; // To bound angle between 0 - 180

    if(angle < viewingAngle) return true;
    else return false;
}

bool Agent::FieldOfVision::canSeeBullet(const Bullet &bullet){
    return false;
}
void Agent::FieldOfVision::updateRays(const float heading, const sf::Vector2f &position){
    // Update ray 1 position
    ray1[0].position = position;
    float rayAngle = (heading - viewingAngle)*DEG2RAD;
    sf::Vector2f endPoint1(
                position.x + std::sin(rayAngle)*viewingDistance,
                position.y - std::cos(rayAngle)*viewingDistance
                );
    ray1[1].position = endPoint1;
    // Update ray 2 position
    ray2[0].position = position;
    rayAngle = (heading + viewingAngle)*DEG2RAD;
    sf::Vector2f endPoint2 = sf::Vector2f(
                position.x + std::sin(rayAngle)*viewingDistance,
                position.y - std::cos(rayAngle)*viewingDistance
                );
    ray2[1].position = endPoint2;
}
