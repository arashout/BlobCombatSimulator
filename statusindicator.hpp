#ifndef STATUSINDICATOR_HPP
#define STATUSINDICATOR_HPP

#include <SFML/Graphics.hpp>
#include <Eigen/Core>
#include "parameters.hpp"

class StatusIndicator : public sf::Drawable
{
public:
    StatusIndicator();
    void update(const sf::Vector2f &agentPos, const Eigen::VectorXf &inputVector);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    sf::RectangleShape staminaBar;
    sf::CircleShape seeAgentOrb;
    sf::CircleShape seeBulletOrb;
    sf::CircleShape canShootOrb;
    const float heightMax = agentParams::agentRadius*2;
};

#endif // STATUSINDICATOR_HPP
