#ifndef SFMLVECTOR_HPP
#define SFMLVECTOR_HPP

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include <math.h>
#include "constants.hpp"

class SFMLVector
{
public:
    static sf::Vector2f normalize(const sf::Vector2f &v);
    static float magnitude(const sf::Vector2f &v);
    static float heading(const sf::Vector2f &v);
    static float dot(const sf::Vector2f &v, const sf::Vector2f &u);
    static sf::Vector2f vectorHeading(float direction);
    static sf::Vector2f limit(const sf::Vector2f &v,
                              const float limitMagnitude);
    static bool circToCircCollision(const sf::CircleShape circ1,
                                    const sf::CircleShape circ2);
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
