#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <stdlib.h>     /* srand, rand */

#define PI 3.14159265
#define DEG2RAD PI/180
#define RAD2DEG 180/PI;
// rand() % LASTENUM
// gives us a random edge not including INSIDE!
#define LAST_ENUM 4

enum class EDGE : short {TOP, BOTTOM, RIGHT, LEFT, INSIDE};

// This is for getting rid of compiler warnings!
// It gets optimized during build
#define UNUSED(p) ((p)=(p))

#endif // CONSTANTS_HPP
