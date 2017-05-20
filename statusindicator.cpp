#include "statusindicator.hpp"
#include "constants.hpp"

StatusIndicator::StatusIndicator()
{
    const sf::Vector2f statusBarSize(statusBars::width, heightMax);
    staminaBar.setFillColor(sf::Color::Green);
    staminaBar.setSize(statusBarSize);
}

void StatusIndicator::update(const sf::Vector2f &agentPos, const Eigen::VectorXf &inputVector)
{
    const float &r = agentParams::agentRadius;
    const float &m = statusBars::margin;
    // Update new positions
    staminaBar.setPosition(agentPos.x + r + m, agentPos.y + r);

    // Update fill
    sf::Vector2f fillVector(statusBars::width, heightMax*inputVector(nnParam::staminaIndex));
    staminaBar.setSize(fillVector);
}

void StatusIndicator::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(staminaBar);

    UNUSED(states);
}
