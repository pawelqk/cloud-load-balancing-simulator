#pragma once

#include <cstdint>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"
#include "Cloud/Task.hpp"
#include "Individual.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

struct Parameters
{
    double mutationProbability;
    std::uint32_t populationSize;
    std::uint32_t maxIterations;
    double eliteIndividualsInInitialGenerationRatio;
    double eliteIndividualsInNextGenerationRatio;
};

class GeneticAlgorithmBase : public PolicyBase
{
  protected:
    GeneticAlgorithmBase(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                         const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor,
                         const logger::LoggerPtr &logger);

    NodeToTaskMapping createNewSolution(const TaskPtrVec &tasks);

  private:
    struct BestSolution
    {
        NodeToTaskMapping solution;
        double value;
    };

    struct ParentPair
    {
        Individual left;
        Individual right;
    };

    struct IndividualsSplitByElitism
    {
        std::vector<Individual> elite;
        std::vector<Individual> notElite;
    };

    void generateInitialPopulation(const TaskPtrVec &tasks);
    Individual generateRandomIndividual(const TaskPtrVec &tasks);
    IndividualsSplitByElitism splitIndividualsByElitism(const std::vector<Individual> &individuals,
                                                        const std::vector<Individual>::size_type count);
    void insertNewIndividual(const Individual &individual);
    std::vector<ParentPair> getParentPairs();
    std::vector<Individual> performCrossover(const std::vector<ParentPair> &parentPairs);
    void chooseNextPopulation(const std::vector<Individual> &descendants);
    std::vector<Individual> selectWithRouletteWheel(const std::vector<Individual> &individuals,
                                                    const std::vector<Individual>::size_type count);

    const Parameters parameters;
    const std::shared_ptr<mapping::MappingAssessor> mappingAssessor;
    BestSolution bestSolution;

    std::vector<Individual> population;
};

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
