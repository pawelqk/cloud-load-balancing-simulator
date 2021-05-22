#include "Random.hpp"

#include <algorithm>
#include <list>
#include <tuple>
#include <utility>

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace random
{

Random::Random(const InfrastructureCPtr &infrastructure, const utility::RandomNumberGeneratorPtr &randomNumberGenerator,
               const logger::LoggerPtr &logger)
    : PolicyBase(infrastructure, logger), randomNumberGenerator(randomNumberGenerator)
{
}

NodeToTaskMapping Random::buildNodeToTaskMappingInternal(const TaskPtrVec &tasks)
{
    NodeToTaskMapping solution;

    auto tasksShuffled = tasks;

    std::shuffle(tasksShuffled.begin(), tasksShuffled.end(), *randomNumberGenerator);
    for (auto &&task : tasksShuffled)
    {
        std::vector<NodeId> possibleNodeIds;
        for (auto &&node : infrastructure->getNodes())
        {
            if (node->canTaskFit(task))
                possibleNodeIds.push_back(node->getId());
        }

        std::uniform_int_distribution<> dis(0, possibleNodeIds.size() - 1);
        solution[possibleNodeIds[dis(*randomNumberGenerator)]].push_back(task);
    }

    return adjustSolutionWithExistingTasks(solution);
}

std::string Random::toString() const
{
    return "Random";
}

NodeToTaskMapping Random::adjustSolutionWithExistingTasks(const NodeToTaskMapping &solution)
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

} // namespace random
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
