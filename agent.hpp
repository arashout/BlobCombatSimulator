#ifndef AGENT_HPP
#define AGENT_HPP

#include "entity.hpp"
#include <unordered_map>
#include "bullet.hpp"
#include <SFML/Graphics/Vertex.hpp>
#include <Eigen/Core>
#include <dna.hpp>
#include "fieldofvision.hpp"
#include "statusindicator.hpp"

class Agent : public Entity
{
public:
    Agent(unsigned genNum);
    Agent(unsigned genNum, Agent parent);
    Agent& operator=(const Agent &a){
        // Copy the important stuff!
        dna = a.dna;
        fov = a.fov;
        id = a.id;
        isDead = a.isDead;
        numHits = a.numHits;
        health = a.health;
        return *this;
    }

    bool operator<(const Agent &a) const{
        return computeFitness() < a.computeFitness();
    }
    void mutate(void){ dna.mutate();}
    void update(const sf::RenderWindow &window);
    void express(std::unordered_map<std::string, Bullet> &bulletMap);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    bool canSeeEntity(const Entity &thatEntity) const;
    float computeFitness(void) const;
    void fillInputVector(
            const std::unordered_map<std::string, Agent> &agentMap,
            std::unordered_map<std::string, Bullet> &bulletMap,
            const sf::RenderWindow &window
            );
    void incrementHits();
    //Getters and Setters
    bool hasDied(void) const;
    void setPosition(const sf::Vector2f p);
    void setRotation(const float heading);
    void setId(const std::string newId);
private:
    static long idCount;
    bool isDead;
    bool canShoot;
    float numHits;
    int health;
    float stamina;

    unsigned shotTimer;

    sf::Vector2f lastPosition; // Use this for checking if out of bounds

    void setup(unsigned genNum);
    void applyInputs(std::unordered_map<std::string, Bullet>& bulletMap);
    void thrust(const float direction);
    void rotate(const float direction);
    void shoot(std::unordered_map<std::string, Bullet>& bulletMap, const bool wantsToShoot);

    void checkBullets(std::unordered_map<std::string, Bullet> &bulletMap);
    void checkAgents(const std::unordered_map<std::string, Agent> &agentMap);
    bool hasAgentInSights(const Agent &thatAgent) const;

    float computeNormalizedStamina(void) const;
    sf::Vector2f computeNormalizedPosition(const sf::Vector2f &pos, const float xMax, const float yMax) const;

    sf::CircleShape eye;
    FieldOfVision fov;
    StatusIndicator statusInd;

    DNA dna;
    Eigen::VectorXf inputVector;
    void changeColor();

    void setupStatusBar(void);
    void updateStatusBar(void);
};

#endif // AGENT_HPP
