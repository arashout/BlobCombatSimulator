#ifndef DNA_HPP
#define DNA_HPP

#include "neuralnetwork.hpp"

class DNA
{
public:
    DNA();
    DNA(const DNA &dna);
    DNA getDNA(void) const;
    void setDNA(const DNA dna);
    void mutate(void);
    NeuralNetwork brain;
private:

};

#endif // DNA_HPP
