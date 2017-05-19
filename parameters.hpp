#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <vector>

namespace simParams {
    const unsigned numBatches = 1;
    const unsigned batchSize = 3;
    const unsigned numGenerations = 4;
    const float rankProbability = .3f;
}

namespace gameParams {
    const unsigned normalFrameRate = 60;
    const float maxTime = 15;
    const float gameSpeed = 1;
}

namespace fovParams {
    const float viewingAngle = 30;
    const float sightDistance = 500;
    const float viewingDistance = 400;
}

namespace nnParam {
    const float floatTrue = 1.0f;
    const float floatFalse = 0.0f;

    const unsigned numInputs = 4;
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

    const std::vector<unsigned> topology {numInputs, 1, numOutputs};
}

namespace dnaParam {
    const float mutationChance = 0.1f;
}

namespace agentParams {
    const float thrustVelocity = 3;
    const float rotationVelocity = 5;
    const float agentRadius = 15;
    const unsigned shotChargeFrames = gameParams::normalFrameRate*0.5; // Number of frames
    const float eyeRadius = 3;
    const unsigned numLives = 3;
}

namespace bulletParams {
    const float baseSpeed = 10;
    const float radius = 3;
}

#endif // PARAMETERS_HPP
