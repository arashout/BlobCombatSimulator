#ifndef NNPARAMETERS_HPP
#define NNPARAMETERS_HPP

#include <vector>

#define NUM_INPUTS 6
#define NUM_OUTPUTS 4
namespace nnParam {
    const float floatTrue = 1.0f;
    const float floatFalse = 0.0f;

    const unsigned numInputs = NUM_INPUTS;
    const unsigned short sightsIndex = 0;
    const unsigned short seeAgentIndex = 1;
    const unsigned short seeBulletIndex = 2;
    const unsigned short shotTimerIndex = 3;
    const unsigned short posXIndex = 4;
    const unsigned short posYIndex = 5;

    const unsigned numOutputs = NUM_OUTPUTS;
    const unsigned short shootIndex = 0;
    const unsigned short thrustIndex = 1;
    const unsigned short rotateLeftIndex = 2;
    const unsigned short rotateRightIndex = 3;


    const std::vector<unsigned> topology {NUM_INPUTS, 8, 8, NUM_OUTPUTS};



}
#endif // NNPARAMETERS_HPP
