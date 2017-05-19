#ifndef FIELDOFVISION_HPP
#define FIELDOFVISION_HPP

#include "agent.hpp"

class FieldOfVision : public sf::Drawable{
public:
    FieldOfVision();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(const float heading, const sf::Vector2f &position);
    bool hasAgentInSights(const Agent &thatAgent) const;
    bool canSeeEntity(const Agent &thisAgent, const Entity &thatEntity) const;
private:
    sf::Vertex ray1[2]; // Heading - viewing angle
    sf::Vertex ray2[2]; // Heading + viewing angle
    sf::Vertex raySights[2]; // Sight representing bullet trajectory

    sf::Vector2f curHeadingVector;

    void updateRays(const float heading, const sf::Vector2f &position);
};

#endif // FIELDOFVISION_HPP
