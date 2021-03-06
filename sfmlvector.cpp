#include "sfmlvector.hpp"
//TODO - convert magnitude to magnitudeSquared where possible


/**
 * @brief Convert float direction to normalized vector
 * in the same direction
 * Assuming direction is angle taken from shape.getRotation
 * 0 Deg => -y
 * 90 Deg => +x
 * 180 Deg => y
 * 270 Deg => -x
 * @param direction [Degrees]
 * @return a normalized vector in the direction of heading
 */
sf::Vector2f SFMLVector::vectorHeading(float direction){
    direction = direction * DEG2RAD;
    float x = std::sin(direction);
    float y = -std::cos(direction);
    return sf::Vector2f(x, y);
}

sf::Vector2f SFMLVector::limit(const sf::Vector2f &v, const float limitMagnitude){
    if(magnitude(v) > limitMagnitude) return normalize(v)*limitMagnitude;

    return v;
}
bool SFMLVector::circToCircCollision(const sf::CircleShape circ1,
                                     const sf::CircleShape circ2){
    sf::Vector2f center = circ1.getPosition();
    sf::Vector2f difference = center - circ2.getPosition();
    float difMag = magnitude(difference);
    float sumRadii = circ1.getRadius() + circ2.getRadius();
    if(difMag < sumRadii) return true;
    else return false;
}

sf::Vector2f SFMLVector::getRandVector(const float vMin, const float vMax){
    float x = vMin + static_cast <float> (rand())/
            ( static_cast <float> (RAND_MAX/(vMax-vMin)));
    float y = vMin + static_cast <float> (rand())/
            ( static_cast <float> (RAND_MAX/(vMax-vMin)));
    return sf::Vector2f(x, y);
}

EDGE SFMLVector::getRandEdge(){
    return static_cast<EDGE>(rand() % LAST_ENUM);
}

sf::Vector2f SFMLVector::getRandPosition(const EDGE edge,
                                                 const unsigned xMax,
                                                 const unsigned yMax,
                                                 const float characterLength
                                                 ){
    sf::Vector2f p;
    switch (edge) {
    case EDGE::LEFT:
        p.x = -characterLength;
        p.y = rand() % yMax;
        break;
    case EDGE::RIGHT:
        p.x = xMax + characterLength;
        p.y = rand() % yMax;
        break;
    case EDGE::TOP:
        p.x = rand() % xMax;
        p.y = -characterLength;
        break;
    case EDGE::BOTTOM:
        p.x = rand() % xMax;
        p.y = yMax + characterLength;
        break;
    default:
        break;
    }

    return p;
}

bool SFMLVector::lineCircleCollision(const sf::Vector2f line[], const sf::CircleShape circ){
    const sf::Vector2f &a = line[0]; // Line Start
    const sf::Vector2f &b = line[1]; // Line End
    const sf::Vector2f &c = circ.getPosition(); // Circle Center
    float r = circ.getRadius();

    sf::Vector2f ab = b-a;
    sf::Vector2f ac = c-a;

    if(dot(ab, ac) < 0) return false; // If vectors facing wrong direction

    sf::Vector2f ad = dot(ab, ac)/dot(ab, ab) * ab; 
    if(magnitude(ab) <= magnitude(ad)) return false; // If circ is too far away
    sf::Vector2f d = ad + a;

    float lineCircleDistance = magnitude(d - c);

    if(lineCircleDistance <= r) return true;
    return false;
}

