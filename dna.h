#ifndef DNA_H
#define DNA_H

#include "neuralnetwork.hpp"

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
