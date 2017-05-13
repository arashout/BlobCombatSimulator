#ifndef DNA_H
#define DNA_H

#include "neuralnetwork.hpp"

class DNA
{
public:
    DNA();
    DNA(NeuralNetwork &brain);
    void mutate();
private:

};

#endif // DNA_H
