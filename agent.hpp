#ifndef AGENT_HPP
#define AGENT_HPP

#include "entity.hpp"
#include <unordered_map>
#include "bullet.hpp"
#include <SFML/Graphics/Vertex.hpp>
#include <Eigen/Core>

class Agent : public Entity
{
public:
    Agent(const float agentRadius, const sf::Vector2f position);
    void update(const float dt,const sf::RenderWindow &window);
    void shoot(std::unordered_map<std::string, Bullet>& bulletMap);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    bool hasAgentInSights(const Agent &thatAgent) const;
    bool canSeeEntity(const Entity &thatEntity) const;
    void setId(const std::string newId);
    bool hasDied(void);
    Eigen::VectorXf getInputVector(
            const std::unordered_map<std::string, Agent> &agentMap,
            std::unordered_map<std::string, Bullet> &bulletMap,
            const sf::RenderWindow &window
            );
    void executeInput(std::unordered_map<std::string, Bullet>& bulletMap);
private:
    // For IDs
    static long count;
    // Limiters on ship movement
    const float terminalSpeed = 150;
    // Speed decay per update
    const float velocityDecay = .5;
    // Additional force on keypress
    const float thrustPower = 250;
    const float rotatePower = 250;
    // Statistics
    bool isDead;
    float timeAlive;
    unsigned kills;
    float score;

    // Shooting status
    const float shotChargeTime = .1; // Time [s] to pass before next shot
    float shotTimer;
    bool canShoot;
    bool intendToShoot;

    // Eye aesthetics
    sf::CircleShape eye;
    const float eyeRadius = 3;

    void applyInputs(const float dt);
    void kinematics(const float dt);
    void thrust(const float dt, const float direction);
    void rotate(const float dt, const float direction);

    // Helper methods
    bool nearbyBullets(std::unordered_map<std::string, Bullet> &bulletMap);
    bool nearbyAgents(const std::unordered_map<std::string, Agent> &agentMap);

    // Field of vision
    class FieldOfVision : public sf::Drawable{
    public:
        FieldOfVision(Agent &parentAgent);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void update(const float heading, const sf::Vector2f &position);
        bool hasAgentInSights(const Agent &thisAgent, const Agent &thatAgent) const;
        bool canSeeEntity(const Agent &thisAgent,const Entity &thatEntity) const;
    private:
        sf::Vertex ray1[2]; // Heading - viewing angle
        sf::Vertex ray2[2]; // Heading + viewing angle
        sf::Vertex raySights[2]; // Sight representing bullet trajectory

        const float viewingAngle = 50;
        const float viewingDistance = 400;
        sf::Vector2f curHeadingVector;

        void updateRays(const float heading, const sf::Vector2f &position);
        // Manually give reference to outer agent class
        // TODO - Make this work - as of now the pointer does not work!
        Agent &thisAgent;

    };
    FieldOfVision fov;
};

#endif // AGENT_HPP
