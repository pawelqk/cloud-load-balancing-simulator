#include "Random.hpp"

#include <algorithm>
#include <list>
#include <tuple>
#include <utility>

#include <Utility/RandomNumberGenerator.hpp>

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace random
{

Random::Random(const InfrastructureCPtr &infrastructure, const logger::LoggerPtr &logger)
    : PolicyBase(infrastructure, logger)
{
}

NodeToTaskMapping Random::buildNodeToTaskMappingInternal(const TaskPtrVec &tasks)
{
    NodeToTaskMapping solution;

    auto tasksShuffled = tasks;

    auto &nodes = infrastructure->getNodes();
    for (auto &&node : nodes)
    {
        const auto task = node->getTask();
        if (task != nullptr)
            tasksShuffled.push_back(task);
    }

    std::shuffle(tasksShuffled.begin(), tasksShuffled.end(), utility::RandomNumberGenerator::getInstance());
    for (auto &&task : tasksShuffled)
    {
        std::vector<NodeId> possibleNodeIds;
        for (auto &&node : infrastructure->getNodes())
        {
            if (node->canTaskFit(task))
                possibleNodeIds.push_back(node->getId());
        }

        std::uniform_int_distribution<> dis(0, possibleNodeIds.size() - 1);
        solution[possibleNodeIds[dis(utility::RandomNumberGenerator::getInstance())]].push_back(task);
    }

    return solution;
}

std::string Random::toString() const
{
    return "Random";
}

} // namespace random
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
