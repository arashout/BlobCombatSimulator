#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <vector>

namespace simParams {
    const unsigned numBatches = 10;
    const unsigned batchSize = 3;
    const unsigned numGenerations = 100;
    const float rankProbability = .3f;
}

namespace gameParams {
    const int normalFPS = 30;
    const unsigned framesPerGame = normalFPS*30;
    const unsigned windowWidth = 600;
    const unsigned windowHeight = 600;
}

namespace fovParams {
    const float viewingAngle = 30;
    const float sightDistance = 500;
    const float viewingDistance = 400;
}

namespace nnParam {
    const float floatTrue = 1.0f;
    const float floatFalse = 0.0f;

    const unsigned numInputs = 6;
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

    const std::vector<unsigned> topology {numInputs, 6, 8, 4, numOutputs};
}

namespace dnaParam {
    const float mutationChance = 0.1f;
}

namespace agentParams {
    const float thrustVelocity = 150/gameParams::normalFPS;
    const float rotationVelocity = 100/gameParams::normalFPS;
    const float agentRadius = 15;
    const unsigned shotChargeFrames = gameParams::normalFPS*1; // Number of frames
    const float eyeRadius = 3;

    const int healthMax = 3;

    const float staminaMax = 100.0f;
    const float staminaRegen = .5f;
}

namespace bulletParams {
    const float baseSpeed = 450/gameParams::normalFPS;
    const float radius = 3;
}

namespace statusBars {
    const unsigned width = 5;
}
#endif // PARAMETERS_HPP
