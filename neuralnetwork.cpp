#include "neuralnetwork.hpp"
#include <math.h>
#include <iostream>

float activationFunction(const float x){
    // Note: The function ranges from 0 to 1 like the sigmoid
    return (std::tanh(x) + 1)/2;
}

float activationDerivative(const float x){
    return (1 - std::tanh(x)*std::tanh(x))/2;
}

NeuralNetwork::NeuralNetwork(const std::vector<unsigned> &topology)
{
    numLayers = topology.size();
    // Create vector of layers of neuron
    // Note: Layers are column vectors
    for(size_t i = 0; i < numLayers; i++){
        unsigned sizeLayer = topology[i];

        Eigen::VectorXf currentLayer(sizeLayer);
        layers.push_back(currentLayer);
    }
    // Setup up Weights matrices
    for(size_t i = 0; i < numLayers - 1; i++){
        // Again because of the bias have to add one
        unsigned numNeuronsCurLayer = topology[i] + 1;
        unsigned numNeuronsNextLayer = topology[i+1];
        // Initialize with random weights
        Eigen::MatrixXf w = Eigen::MatrixXf::Random(
                    numNeuronsCurLayer,
                    numNeuronsNextLayer
                    );
        weightsMatrices.push_back(w);
    }
}

void NeuralNetwork::feedforward(const Eigen::VectorXf &inputs){
    // Set input layer to have given input values
    layers[0] = inputs;
    // Implement feed forward
    for(size_t i = 0; i < numLayers - 1; i++){
        Eigen::VectorXf &curLayer = layers[i];
        Eigen::VectorXf &nextLayer = layers[i+1];
        Eigen::VectorXf layerWithBias(curLayer.rows() + 1);
        Eigen::MatrixXf &curWeights = weightsMatrices[i];
        // Sum weighted neuron values from current layer to next layer
        // NOTE: That bias is last neuron in layer
        layerWithBias << curLayer, Eigen::VectorXf::Constant(1, 1.0);
        nextLayer = layerWithBias.transpose() * curWeights;
        // Activation function
        nextLayer = nextLayer.unaryExpr(std::ptr_fun(activationFunction));
    }
}

void NeuralNetwork::displayLayers(void) const{
    for(auto&& v : layers){
        std::cout << v << std::endl << "____" << std::endl;
    }
}

void NeuralNetwork::displayWeights(void) const{
    for(auto&& w : weightsMatrices){
        std::cout << w << std::endl << "____" << std::endl;
    }
}

void NeuralNetwork::setWeights(const unsigned matrixIndex, const Eigen::MatrixXf &newWeights){
    weightsMatrices[matrixIndex] = newWeights;
}

Eigen::MatrixXf NeuralNetwork::getWeights(const unsigned matrixIndex){
    return weightsMatrices[matrixIndex];
}
