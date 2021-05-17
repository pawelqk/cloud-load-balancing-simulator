#pragma once

#include <cstdint>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"
#include "Cloud/Task.hpp"
#include "GeneticAlgorithmBase.hpp"
#include "Individual.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

class OnlineGeneticAlgorithm : public PolicyBase
{
  public:
    OnlineGeneticAlgorithm(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                           const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor,
                           const logger::LoggerPtr &logger);

    std::string toString() const override;

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

    NodeToTaskMapping buildNodeToTaskMappingInternal(const TaskPtrVec &tasks) override;
    NodeToTaskMapping createNewSolution(const TaskPtrVec &tasks);
    NodeToTaskMapping adjustSolutionWithExistingTasks(const NodeToTaskMapping &solution);
    BestSolution generateInitialPopulation(const TaskPtrVec &tasks);
    Individual generateRandomIndividual(const TaskPtrVec &tasks);
    std::vector<ParentPair> getParentPairs();
    std::vector<Individual> performCrossover(const std::vector<ParentPair> &parentPairs);
    void chooseNextPopulation(const std::vector<Individual> &descendants);
    std::vector<Individual> selectWithRouletteWheel(const std::vector<Individual> &individuals,
                                                    const std::vector<Individual>::size_type count);

    const Parameters parameters;
    const std::shared_ptr<mapping::MappingAssessor> mappingAssessor;

    std::vector<Individual> population;
};

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
