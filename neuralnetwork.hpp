#ifndef NEURALNETWORK_HPP
#define NEURALNETWORK_HPP

#include <Eigen/Core>
#include <vector>
class NeuralNetwork
{
public:
    NeuralNetwork(const std::vector<unsigned> &topology);
    Eigen::VectorXf feedforward(const Eigen::VectorXf &inputs);
    void setWeights(const unsigned matrixIndex, const Eigen::MatrixXf &newWeights);
    Eigen::MatrixXf getWeights(const unsigned matrixIndex);
    void displayWeights(void) const;
    void displayLayers(void) const;
private:
    unsigned numLayers;
    std::vector<Eigen::VectorXf> layers;
    std::vector<Eigen::MatrixXf> weightsMatrices;
    void applyActivation(const unsigned layerNum);
};

#endif // NEURALNETWORK_HPP
