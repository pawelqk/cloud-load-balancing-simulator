#include "OnlineGeneticAlgorithmWithMigrationsAndPreemptions.hpp"

#include <utility>

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

OnlineGeneticAlgorithmWithMigrationsAndPreemptions::OnlineGeneticAlgorithmWithMigrationsAndPreemptions(
    const InfrastructureCPtr &infrastructure, const Parameters &parameters,
    const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor, const logger::LoggerPtr &logger)
    : GeneticAlgorithmBase(infrastructure, parameters, mappingAssessor, logger)
{
}

NodeToTaskMapping OnlineGeneticAlgorithmWithMigrationsAndPreemptions::buildNodeToTaskMappingInternal(
    const TaskPtrVec &tasks)
{
    auto allTasks = tasks;
    for (auto &&node : infrastructure->getNodes())
    {
        const auto task = node->getTask();
        if (task != nullptr)
            allTasks.push_back(task);
    }

    return createNewSolution(allTasks);
}

std::string OnlineGeneticAlgorithmWithMigrationsAndPreemptions::toString() const
{
    return "OnlineGeneticAlgorithmWithMigrationsAndPreemptions";
}

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
