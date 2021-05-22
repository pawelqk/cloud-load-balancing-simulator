#include "OnlineGeneticAlgorithm.hpp"

#include <cmath>
#include <iterator>
#include <optional>
#include <random>

#include "Utility/RandomNumberGenerator.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

OnlineGeneticAlgorithm::OnlineGeneticAlgorithm(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                                               const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor,
                                               const logger::LoggerPtr &logger,
                                               const utility::RandomNumberGeneratorPtr &randomNumberGenerator)
    : GeneticAlgorithmBase(infrastructure, parameters, mappingAssessor, logger, randomNumberGenerator)
{
}

std::string OnlineGeneticAlgorithm::toString() const
{
    return "OnlineGeneticAlgorithm";
}

NodeToTaskMapping OnlineGeneticAlgorithm::buildNodeToTaskMappingInternal(const TaskPtrVec &tasks)
{
    return adjustSolutionWithExistingTasks(createNewSolution(tasks));
}

NodeToTaskMapping OnlineGeneticAlgorithm::adjustSolutionWithExistingTasks(const NodeToTaskMapping &solution)
{
    auto adjustedSolution = solution;

    const auto &nodes = infrastructure->getNodes();
    for (auto &&node : nodes)
    {
        const auto task = node->getTask();
        if (task != nullptr)
            adjustedSolution[node->getId()].push_front(task);
    }

    return adjustedSolution;
}

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
