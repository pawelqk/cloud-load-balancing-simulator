#pragma once

#include <cstdint>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"
#include "Cloud/Task.hpp"
#include "Individual.hpp"
#include "Utility/RandomNumberGenerator.hpp"

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

std::string toString(const Parameters &parameters);

class GeneticAlgorithmBase : public PolicyBase
{
  protected:
    GeneticAlgorithmBase(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                         const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor,
                         const logger::LoggerPtr &logger,
                         const utility::RandomNumberGeneratorPtr &randomNumberGenerator);

    NodeToTaskMapping createNewSolution(const TaskPtrVec &tasks);

    const std::shared_ptr<mapping::MappingAssessor> mappingAssessor;
    const utility::RandomNumberGeneratorPtr randomNumberGenerator;

  private:
    struct BestSolution
    {
        NodeToTaskMapping solution;
        double value;
    };

    struct ParentPair
    {
        IndividualPtr left;
        IndividualPtr right;
    };

    struct IndividualsSplitByElitism
    {
        std::vector<IndividualPtr> elite;
        std::vector<IndividualPtr> notElite;
    };

    void generateInitialPopulation(const TaskPtrVec &tasks);
    virtual IndividualPtr generateRandomIndividual(const TaskPtrVec &tasks);
    IndividualsSplitByElitism splitIndividualsByElitism(const std::vector<IndividualPtr> &individuals,
                                                        const std::vector<IndividualPtr>::size_type count);
    void insertNewIndividual(const IndividualPtr &individual);
    std::vector<ParentPair> getParentPairs();
    std::vector<IndividualPtr> performCrossover(const std::vector<ParentPair> &parentPairs);
    void chooseNextPopulation(const std::vector<IndividualPtr> &descendants);
    std::vector<IndividualPtr> selectWithRouletteWheel(const std::vector<IndividualPtr> &individuals,
                                                       const std::vector<IndividualPtr>::size_type count);
    std::vector<IndividualPtr> selectUniqueWithRouletteWheel(const std::vector<IndividualPtr> &individuals,
                                                             const std::vector<IndividualPtr>::size_type count);

    const Parameters parameters;
    BestSolution bestSolution;

    std::vector<IndividualPtr> population;
};

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
