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
        health = a.health;
        return *this;
    }

    bool operator<(const Agent &a) const{
        return getHealth() < a.getHealth();
    }
    void mutate(void){ dna.mutate();}
    void update(const sf::RenderWindow &window);
    void express(std::unordered_map<std::string, Bullet> &bulletMap);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    bool canSeeEntity(const Entity &thatEntity) const;

    void setInputs(
            const std::unordered_map<std::string, Agent> &agentMap,
            std::unordered_map<std::string, Bullet> &bulletMap,
            const sf::RenderWindow &window
            );
    void incrementHealth();
    //Getters and Setters
    bool hasDied(void) const;
    void setPosition(const sf::Vector2f p);
    void setRotation(const float heading);
    void setId(const std::string newId);
    int getHealth(void) const;
private:
    static long idCount;
    bool isDead;
    bool canShoot;
    float health;
    float stamina;

    sf::CircleShape eye;
    FieldOfVision fov;
    StatusIndicator statusInd;

    DNA dna;
    Eigen::VectorXf inputVector;

    unsigned shotTimer;

    sf::Vector2f lastPosition; // Use this for reverting position

    void setup(unsigned genNum);
    void applyInputs(std::unordered_map<std::string, Bullet>& bulletMap);
    void thrust(void);
    void rotate(const short direction);
    void shoot(std::unordered_map<std::string, Bullet>& bulletMap);

    void checkBullets(std::unordered_map<std::string, Bullet> &bulletMap);
    void checkAgents(const std::unordered_map<std::string, Agent> &agentMap);

    // Helpers
    float computeNormalizedStamina(void) const;
    sf::Vector2f computeNormalizedPosition(const sf::Vector2f &pos, const float xMax, const float yMax) const;
    float computeNormalizedViewAngle(void) const;
    bool passStaminaCheck(const float cost);
    float computeNormalizedHealth() const;
};

#endif // AGENT_HPP
