#ifndef AGENT_HPP
#define AGENT_HPP

#include "entity.hpp"
#include <unordered_map>
#include "bullet.hpp"
#include <SFML/Graphics/Vertex.hpp>
#include <Eigen/Core>
#include <dna.h>

class Agent : public Entity
{
public:
    Agent(unsigned genNum);

    Agent& operator=(const Agent a){
        // Copy the important stuff!
        dna = a.dna;
        isDead = a.isDead;
        timeAlive = a.timeAlive;
        kills = a.kills;
        return *this;
    }

    bool operator<(const Agent a) const{
        return computeFitness() < a.computeFitness();
    }

    void update(const float dt, const sf::RenderWindow &window);
    void express(const float dt, std::unordered_map<std::string, Bullet> &bulletMap);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void setId(const std::string newId);
    void setPosition(const sf::Vector2f p);
    void setRotation(const float heading);
    static void resetIdCount(void);
    bool hasDied(void) const;
    void incrementKillCount(void);
    float computeFitness(void) const;
    void fillInputVector(
            const std::unordered_map<std::string, Agent> &agentMap,
            std::unordered_map<std::string, Bullet> &bulletMap,
            const sf::RenderWindow &window
        );
private:
    static long idCount;
    bool isDead;
    float timeAlive;
    unsigned kills;

    float shotTimer;
    bool canShoot;

    void applyInputs(const float dt, std::unordered_map<std::string, Bullet>& bulletMap);
    void kinematics(const float dt);
    void thrust(const float dt, const float direction);
    void rotate(const float dt, const float direction);
    void shoot(std::unordered_map<std::string, Bullet>& bulletMap, const bool wantsToShoot);

    void checkBullets(std::unordered_map<std::string, Bullet> &bulletMap);
    void checkAgents(const std::unordered_map<std::string, Agent> &agentMap);
    bool hasAgentInSights(const Agent &thatAgent) const;
    bool canSeeEntity(const Entity &thatEntity) const;

    float computeNormalizedShotTimer(void) const;
    sf::Vector2f computeNormalizedPosition(const sf::Vector2f &pos, const float xMax, const float yMax) const;

    sf::CircleShape eye;
    class FieldOfVision : public sf::Drawable{
    public:
        FieldOfVision(Agent &parentAgent);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void update(const float heading, const sf::Vector2f &position);
        bool hasAgentInSights(const Agent &thatAgent) const;
        bool canSeeEntity(const Agent &thisAgent,const Entity &thatEntity) const;
    private:
        sf::Vertex ray1[2]; // Heading - viewing angle
        sf::Vertex ray2[2]; // Heading + viewing angle
        sf::Vertex raySights[2]; // Sight representing bullet trajectory

        sf::Vector2f curHeadingVector;

        void updateRays(const float heading, const sf::Vector2f &position);
        // Manually give reference to outer agent class
        // TODO - Make this work - as of now the pointer does not work!
        Agent &thisAgent;

    };
    FieldOfVision fov;

    DNA dna;
    Eigen::VectorXf inputVector;

};

#endif // AGENT_HPP
