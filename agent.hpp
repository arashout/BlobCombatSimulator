#ifndef AGENT_HPP
#define AGENT_HPP

#include "entity.hpp"
#include <unordered_map>
#include "bullet.hpp"
#include <SFML/Graphics/Vertex.hpp>
#include <Eigen/Core>
#include <dna.hpp>
#include "fieldofvision.hpp"

class Agent : public Entity
{
public:
    Agent(unsigned genNum);
    Agent(unsigned genNum, Agent parent);
    Agent& operator=(const Agent &a){
        // Copy the important stuff!
        dna = a.dna;
        fov = a.fov;
        isDead = a.isDead;
        numHits = a.numHits;
        numHitten = a.numHitten;
        return *this;
    }

    bool operator<(const Agent &a) const{
        return computeFitness() < a.computeFitness();
    }
    void mutate(void){ dna.mutate();}
    void update(const sf::RenderWindow &window);
    void express(std::unordered_map<std::string, Bullet> &bulletMap);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void setId(const std::string newId);
    void setPosition(const sf::Vector2f p);
    void setRotation(const float heading);
    bool canSeeEntity(const Entity &thatEntity) const;
    static void resetIdCount(void);
    bool hasDied(void) const;
    void incrementKillCount(void);
    float computeFitness(void) const;
    void fillInputVector(
            const std::unordered_map<std::string, Agent> &agentMap,
            std::unordered_map<std::string, Bullet> &bulletMap,
            const sf::RenderWindow &window
            );
    void incrementHits();
private:
    static long idCount;
    bool isDead;
    unsigned numHits;
    unsigned numHitten;

    unsigned shotTimer;
    bool canShoot;

    void setup(unsigned genNum);
    void applyInputs(std::unordered_map<std::string, Bullet>& bulletMap);
    void thrust(const float direction);
    void rotate(const float direction);
    void shoot(std::unordered_map<std::string, Bullet>& bulletMap, const bool wantsToShoot);

    void checkBullets(std::unordered_map<std::string, Bullet> &bulletMap);
    void checkAgents(const std::unordered_map<std::string, Agent> &agentMap);
    bool hasAgentInSights(const Agent &thatAgent) const;

    float computeNormalizedShotTimer(void) const;
    sf::Vector2f computeNormalizedPosition(const sf::Vector2f &pos, const float xMax, const float yMax) const;

    sf::CircleShape eye;
    FieldOfVision fov;

    DNA dna;
    Eigen::VectorXf inputVector;

};

#endif // AGENT_HPP
