#include "differentialevolver.h"
#include "util.h"
#include <unordered_set>
#include <algorithm>
#include <limits>
#include <iostream>

DifferentialEvolver::DifferentialEvolver(double crossoverRate, double scalingFactor)
    : crossoverRate(crossoverRate), scalingFactor(scalingFactor)
{

}

void DifferentialEvolver::initialize(unsigned int popSize, unsigned int dimensionality,
                                     double min, double max, const Individual& prefix)
{
    this->dimensionality = dimensionality;
    this->prefixLength = prefix.size();
    for (unsigned int i = 0; i < popSize; i++) {
        Individual ind = prefix;

        for (unsigned int j = prefixLength; j < dimensionality; j++) {
            ind.push_back(Util::random(min, max));
        }

        std::cout << ind[0] << ", " << ind[1] << "\n";
        population.push_back(ind);
        fitnesses.push_back(0);
    }
}

void DifferentialEvolver::setObjectiveFunction(DifferentialEvolver::ObjectiveFunction function)
{
    this->objectiveFunction = function;
}

void DifferentialEvolver::improve()
{
    for (unsigned int i = 0; i < population.size(); i++) {
        std::unordered_set<unsigned> agentSet = Util::chooseRandomlyRestricted(population.size(), 3, {i});
        std::vector<unsigned> agents(agentSet.begin(), agentSet.end());

        const Individual& a = population[agents[0]];
        const Individual& b = population[agents[1]];
        const Individual& c = population[agents[2]];
        Individual candidate = population[i];

        unsigned int R = Util::irandom(prefixLength, dimensionality - 1);

        for (unsigned int k = prefixLength; k < dimensionality; k++) {
            double r = Util::random();
            if (r < crossoverRate || k == R) {
                candidate[k] = a[k] + scalingFactor * (b[k] - c[k]);
            }
        }

        double candidateQuality = objectiveFunction(candidate);

        if (candidateQuality > fitnesses[i]) {
            population[i] = candidate;
            fitnesses[i] = candidateQuality;
        }
    }
}

const std::vector<DifferentialEvolver::Individual>& DifferentialEvolver::getPopulation() const
{
    return population;
}

const DifferentialEvolver::Individual& DifferentialEvolver::getBestIndividual() const
{
    auto it = std::max_element(fitnesses.begin(), fitnesses.end());
    unsigned int index = std::distance(fitnesses.begin(), it);

    return population[index];
}

double DifferentialEvolver::getFitness(unsigned int index) const
{
    return fitnesses[index];
}
