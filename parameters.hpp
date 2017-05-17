#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#define NUM_INPUTS 6
#define NUM_OUTPUTS 4

#include <vector>

namespace simParams {
    const unsigned numBatches = 3;
    const unsigned batchSize = 4;
    const float maxTime = 10;
    const float gameSpeed = 1;
}

namespace agentParams {
    const float terminalSpeed = 150;
    const float velocityDecay = .5;
    const float thrustPower = 200;
    const float rotatePower = 200;
    const float agentRadius = 15;
    const float shotChargeTime = 1; // Time [s] to pass before next shot
    const float eyeRadius = 3;
}

namespace fovParams {
    const float viewingAngle = 50;
    const float viewingDistance = 300;
}

namespace nnParam {
    const float floatTrue = 1.0f;
    const float floatFalse = 0.0f;

    const unsigned numInputs = 6 ;
    const unsigned short sightsIndex = 0;
    const unsigned short seeAgentIndex = 1;
    const unsigned short seeBulletIndex = 2;
    const unsigned short shotTimerIndex = 3;
    const unsigned short posXIndex = 4;
    const unsigned short posYIndex = 5;

    const unsigned numOutputs = 4;
    const unsigned short shootIndex = 0;
    const unsigned short thrustIndex = 1;
    const unsigned short rotateLeftIndex = 2;
    const unsigned short rotateRightIndex = 3;

    const std::vector<unsigned> topology {numInputs, 8, 8, numOutputs};
}

namespace dnaParam {
    const float mutationChance = 0.05f;
}

namespace bulletParams {
    const float baseSpeed = 300;
    const float radius = 3;
}

#endif // PARAMETERS_HPP
