#include "statusindicator.hpp"
#include "constants.hpp"

StatusIndicator::StatusIndicator()
{
    const sf::Vector2f statusBarSize(statusParams::width, heightMax);
    staminaBar.setRotation(180);
    staminaBar.setFillColor(sf::Color::Green);
    staminaBar.setSize(statusBarSize);

    // TODO: List based orbs? Hard-coding is annoying
    seeAgentOrb.setRadius(statusParams::orbRadius);
    seeBulletOrb.setRadius(statusParams::orbRadius);
    canShootOrb.setRadius(statusParams::orbRadius);
}

void StatusIndicator::update(const sf::Vector2f &agentPos, const Eigen::VectorXf &inputVector)
{
    const float r = agentParams::agentRadius;
    const float ro = statusParams::orbRadius;
    const float mx = statusParams::xMargin;
    const float my = statusParams::yMargin;
    // Update new positions
    staminaBar.setPosition(agentPos.x + r + mx, agentPos.y + r);

    const sf::Vector2f orbPos(agentPos.x - r, agentPos.y - (r + my));
    seeAgentOrb.setPosition(orbPos);
    seeBulletOrb.setPosition(orbPos.x + 2*ro, orbPos.y);
    canShootOrb.setPosition(orbPos.x + 4*ro, orbPos.y);

    // Update fill
    sf::Vector2f fillVector(statusParams::width, heightMax*inputVector(nnParam::staminaIndex));
    staminaBar.setSize(fillVector);

    // Orbs default to Black
    if(inputVector(nnParam::seeAgentIndex) == nnParam::floatTrue) {
        seeAgentOrb.setFillColor(statusParams::seeAgentColor);
    }
    else seeAgentOrb.setFillColor(sf::Color::Black);

    if(inputVector(nnParam::seeBulletIndex) == nnParam::floatTrue) {
        seeBulletOrb.setFillColor(sf::Color::Blue);
    }
    else seeBulletOrb.setFillColor(sf::Color::Black);


    if(inputVector(nnParam::canShootIndex) == nnParam::floatTrue) {
        canShootOrb.setFillColor(sf::Color::Magenta);
    }
    else canShootOrb.setFillColor(sf::Color::Black);
}

void StatusIndicator::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(staminaBar);
    target.draw(seeAgentOrb);
    target.draw(seeBulletOrb);
    target.draw(canShootOrb);

    UNUSED(states);
}
