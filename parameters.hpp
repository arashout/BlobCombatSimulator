#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <SFML/Graphics/Color.hpp>
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
    const float viewingAngle = 60;
    const float sightDistance = 150;
    const float viewingDistance = 300;
}

namespace nnParam {
    const float floatTrue = 1.0f;
    const float floatFalse = 0.0f;

    const unsigned numInputs = 7;
    const unsigned short sightsIndex = 0;
    const unsigned short seeAgentIndex = 1;
    const unsigned short seeBulletIndex = 2;
    const unsigned short canShootIndex = 3;
    const unsigned short posXIndex = 4;
    const unsigned short posYIndex = 5;
    const unsigned short staminaIndex = 6;

    const unsigned numOutputs = 4;
    const unsigned short shootIndex = 0;
    const unsigned short thrustIndex = 1;
    const unsigned short rotateLeftIndex = 2;
    const unsigned short rotateRightIndex = 3;

    const std::vector<unsigned> topology {numInputs, 10, 10, numOutputs};
}

namespace dnaParam {
    const float mutationChance = 0.25f;
}

namespace agentParams {
    const float thrustVelocity = 150/gameParams::normalFPS;
    const float rotationVelocity = 100/gameParams::normalFPS;
    const float agentRadius = 15;
    const unsigned shotChargeFrames = gameParams::normalFPS*.1; // Number of frames
    const float eyeRadius = 3;

    const int healthMax = 3;

    const float rotationCost = .5f;
    const float thrustCost = .5f;
    const float shootCost = 10;
    const float staminaMax = 100.0f;
    const float staminaRegen = .3f;
}

namespace bulletParams {
    const float baseSpeed = 450/gameParams::normalFPS;
    const float radius = 3;
}

namespace statusParams {
    const unsigned width = 5;
    const float xMargin = 5;
    const float yMargin = 10;
    const float orbRadius = 5;
    const sf::Color seeAgentColor(255, 100, 0);

}
#endif // PARAMETERS_HPP
