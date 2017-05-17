#ifndef NEURALNETWORK_HPP
#define NEURALNETWORK_HPP

#include <Eigen/Core>
#include <vector>

class NeuralNetwork
{
public:
    NeuralNetwork(const std::vector<unsigned> &topology);
    NeuralNetwork(const NeuralNetwork &nn);
    void feedforward(const Eigen::VectorXf &inputs);
    Eigen::VectorXf computePrediction(void) const;

    void displayWeights(void) const;
    void displayLayers(void) const;

    unsigned getNumLayers() const;
    std::vector<Eigen::MatrixXf> getWeightsMatrices() const;
    void setWeightsMatrices(const std::vector<Eigen::MatrixXf> &newWeights);

private:
    unsigned numLayers;
    std::vector<Eigen::VectorXf> layers;
    std::vector<Eigen::MatrixXf> weightsMatrices;
    void applyActivation(const unsigned layerNum);
};

#endif // NEURALNETWORK_HPP
