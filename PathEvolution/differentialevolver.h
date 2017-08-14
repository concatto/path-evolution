#ifndef DIFFERENTIALEVOLVER_H
#define DIFFERENTIALEVOLVER_H

#include <vector>

class DifferentialEvolver
{
public:
    using Individual = std::vector<double>;
    DifferentialEvolver(double crossoverRate, double scalingFactor);

    void initialize(unsigned int popSize, unsigned int dimensionality, double min, double max);
    const std::vector<Individual>& getPopulation() const;

private:
    unsigned int dimensionality;
    double crossoverRate;
    double scalingFactor;
    std::vector<Individual> population;
};

#endif // DIFFERENTIALEVOLVER_H
