#include "dna.h"
#include "nnparameters.hpp"

DNA::DNA() : brain(nnParam::topology)
{
}

DNA::DNA(NeuralNetwork &nn) : brain(nnParam::topology){
}
