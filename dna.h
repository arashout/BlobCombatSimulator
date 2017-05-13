#ifndef DNA_H
#define DNA_H

#include "neuralnetwork.hpp"

const std::vector<unsigned> topology {5, 15, 5};

class DNA
{
public:
    DNA();
    DNA(NeuralNetwork &nn);
    void mutate();
private:
    NeuralNetwork brain;
};

#endif // DNA_H
