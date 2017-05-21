#ifndef FIELDOFVISION_HPP
#define FIELDOFVISION_HPP

#include "agent.hpp"

class FieldOfVision : public sf::Drawable{
public:
    FieldOfVision();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(const float heading, const sf::Vector2f &position);
    bool canSeeEntity(const Agent &thisAgent, const Entity &thatEntity) const;
    float getViewAngle(void) const;
    void setViewAngle(const float newViewAngle);

private:
    sf::Vertex ray1[2]; // Heading - viewing angle
    sf::Vertex ray2[2]; // Heading + viewing angle

    sf::Vector2f curHeadingVector;
    float viewAngle;

    void updateRays(const float heading, const sf::Vector2f &position);
};

#endif // FIELDOFVISION_HPP
