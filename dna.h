#ifndef DNA_H
#define DNA_H

#include "neuralnetwork.hpp"

class DNA
{
public:
    DNA();
    DNA(NeuralNetwork &nn);
    DNA getDNA(void) const;
    void setDNA(const DNA dna);
    void mutate(void);
    NeuralNetwork brain;
private:

};

#endif // DNA_H
