#include "dna.h"
#include "parameters.hpp"
#include <iostream>

float randMToN(float M, float N)
{
    return M + (rand() / ( RAND_MAX / (N-M) ) ) ;
}

DNA::DNA() : brain(nnParam::topology)
{
}

DNA::DNA(NeuralNetwork &nn) : brain(nnParam::topology){

}

DNA DNA::getDNA(void) const{
    DNA dnaReplica = *this;
    return dnaReplica;
}

void DNA::mutate(void){
    std::vector<Eigen::MatrixXf> tempWeights = brain.getWeightsMatrices();
    for(size_t i = 0; i < tempWeights.size(); i++){
        Eigen::MatrixXf &matrix = tempWeights[i];
        unsigned nRows = matrix.rows();
        unsigned nCols = matrix.cols();
        for (size_t j = 0; j < nCols; j++){
            for (size_t k = 0; k < nRows; k++){
                // Number between 0 - 1
                float randVal = randMToN(0.0f, 1.0f);
                if(randVal < dnaParam::mutationChance){
                    float mutationAmount = randMToN(-1.0f, 1.0f);
                    matrix(k,j) += mutationAmount;
                }
            }
        }
    }
    brain.setWeightsMatrices(tempWeights);
}
