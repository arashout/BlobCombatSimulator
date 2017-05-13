#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "SFML/Graphics.hpp"
#include "constants.hpp"
#include <string>
#include <iostream>
#include <assert.h>

/**
 * @brief The Entity class
 * Any object that moves on the screen will derive
 * from the entity object
 */
class Entity : public sf::Drawable
{
public:
    // Pure virtual function to be overrided (Thus the "= 0")
    virtual void update(const float dt,const sf::RenderWindow &window) = 0;
    // Allows us to do "window.draw(child of Entity);"
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(shape);
        UNUSED(states);
    }

    virtual std::string getId(void) const{return id;}

    virtual EDGE outOfBounds(const sf::RenderWindow& window,
                             float characterLength) const
    {
        sf::Vector2f position = shape.getPosition();
        // Ensure that entity is completely out of bounds THUS
        // To check if completely out of bounds on LEFT EDGE check "rightMost" point
        float rightMost = position.x + characterLength;
        float leftMost = position.x - characterLength;
        float bottomMost = position.y + characterLength;
        float topMost = position.y - characterLength;
        if(rightMost < 0) return EDGE::LEFT;
        else if(bottomMost < 0) return EDGE::TOP;
        else if(leftMost > window.getSize().x) return EDGE::RIGHT;
        else if(topMost > window.getSize().y) return EDGE::BOTTOM;
        else return EDGE::INSIDE;
    }
    virtual void resetPosition(const EDGE edge,
                               const sf::RenderWindow& window,
                               const float characterLength)
    {
        sf::Vector2f position = shape.getPosition();
        // Switch statement that resets the entity position based on which "edge"
        // it has exited from
        switch(edge){
        case EDGE::TOP:
            shape.setPosition(position.x,  window.getSize().y + characterLength);
            break;
        case EDGE::BOTTOM:
            shape.setPosition(position.x, -characterLength);
            break;
        case EDGE::LEFT:
            shape.setPosition(window.getSize().x + characterLength, position.y);
            break;
        case EDGE::RIGHT:
            shape.setPosition(-characterLength, position.y);
            break;
        default:
            // Do Nothing
            break;
        }
    }
    virtual sf::CircleShape getShape(void) const {return shape;}
    virtual sf::Vector2f getPosition(void) const {return shape.getPosition();}
    virtual bool operator == (const Entity &e) const {
        return id == e.getId();
     }
    virtual bool operator != (const Entity &e) const {
        return id != e.getId();
     }

// Protected so that child classes have access to these
protected:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    std::string id;
    virtual void kinematics(const float dt) = 0;

};

#endif // ENTITY_HPP
