#include "differentialevolver.h"
#include "util.h"

DifferentialEvolver::DifferentialEvolver(double crossoverRate, double scalingFactor)
    : crossoverRate(crossoverRate), scalingFactor(scalingFactor)
{

}

void DifferentialEvolver::initialize(unsigned int popSize, unsigned int dimensionality, double min, double max)
{
    this->dimensionality = dimensionality;
    for (unsigned int i = 0; i < popSize; i++) {
        Individual ind;

        for (unsigned int j = 0; j < dimensionality; j++) {
            ind.push_back(Util::random(min, max));
        }

        population.push_back(ind);
    }
}

const std::vector<DifferentialEvolver::Individual>& DifferentialEvolver::getPopulation() const
{
    return population;
}
