#ifndef SFMLVECTOR_HPP
#define SFMLVECTOR_HPP

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include <math.h>
#include "constants.hpp"

class SFMLVector
{
public:
    inline static sf::Vector2f normalize(const sf::Vector2f &v){
        float vSqrt = std::sqrt(v.x*v.x + v.y*v.y);
        return sf::Vector2f(v/vSqrt);
    }

    inline static float magnitude(const sf::Vector2f &v){
        return std::sqrt(v.x*v.x + v.y*v.y);
    }

    inline static float heading(const sf::Vector2f &v){
        // Shifted 90 degrees since SFML 0 degree == UP
        return std::atan(v.y/v.x) * DEG2RAD - 90;
    }

    inline static float dot(const sf::Vector2f &v, const sf::Vector2f &u){
        return v.x*u.x + v.y*u.y;
    }

    inline static sf::Vector2f projection(const sf::Vector2f &a, const sf::Vector2f &b){
        return dot(a,b)/dot(a,b)*a;
    }

    static sf::Vector2f vectorHeading(float direction);
    static sf::Vector2f limit(const sf::Vector2f &v,
                              const float limitMagnitude);

    static bool circToCircCollision(const sf::CircleShape circ1,
                                    const sf::CircleShape circ2);

    static bool lineCircleCollision(const sf::Vector2f line[],
                                    const sf::CircleShape circ);

    static bool circToTriCollision(const sf::CircleShape circle,
                                   const sf::CircleShape triangle);
    static sf::Vector2f getRandVector(const float min, const float max);


    // Not really related to SFML but don't know where else to put them
    static EDGE getRandEdge();
    /**
     * @brief Will return a random position vector right outside the window
     * on the intended edge
     * @param edge
     * @param xMax - Limit in the x-direction, assume min=0
     * @param yMax - Limit in the y-direction, assume min=0
     * @param characterLength
     * @return
     */
    static sf::Vector2f getRandPosition(const EDGE edge,
                                                const unsigned xMax,
                                                const unsigned yMax,
                                                const float characterLength
                                                );

};

#endif // SFMLVECTOR_HPP
