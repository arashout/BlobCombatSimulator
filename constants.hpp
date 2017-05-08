#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <stdlib.h>     /* srand, rand */

#define PI 3.14159265
#define DEG2RAD PI/180
#define RAD2DEG 180/PI;
// rand() % LASTENUM
// gives us a random edge not including INSIDE!
#define LASTENUM 4

enum class EDGE : short {TOP, BOTTOM, RIGHT, LEFT, INSIDE};

#endif // CONSTANTS_HPP
