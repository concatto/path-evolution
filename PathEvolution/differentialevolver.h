#ifndef DIFFERENTIALEVOLVER_H
#define DIFFERENTIALEVOLVER_H

#include <vector>
#include <functional>
#include <utility>

class DifferentialEvolver
{
public:
    using Individual = std::vector<double>;
    using ObjectiveFunction = std::function<double(const Individual&)>;

    DifferentialEvolver(double crossoverRate, double scalingFactor);

    void initialize(unsigned int popSize, unsigned int dimensionality,
                    double min, double max, const Individual &prefix = Individual(), const Individual& suffix = Individual());
    void setObjectiveFunction(ObjectiveFunction function);
    void improve();

    const std::vector<Individual>& getPopulation() const;
    const DifferentialEvolver::Individual &getBestIndividual() const;
    double getFitness(unsigned int index) const;
private:
    unsigned int dimensionality;
    unsigned int prefixLength;
    unsigned int suffixLength;
    double crossoverRate;
    double scalingFactor;
    std::vector<Individual> population;
    std::vector<double> fitnesses;
    ObjectiveFunction objectiveFunction;
};

#endif // DIFFERENTIALEVOLVER_H
