#include "ShortestRemainingTimeFirst.hpp"

#include <algorithm>
#include <cstdint>
#include <iterator>

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace shortestremainingtimefirst
{

ShortestRemainingTimeFirst::ShortestRemainingTimeFirst(const InfrastructureCPtr &infrastructure,
                                                       const logger::LoggerPtr &logger)
    : PolicyBase(infrastructure, logger)
{
}

NodeToTaskMapping ShortestRemainingTimeFirst::buildNodeToTaskMapping(const TaskPtrVec &tasks)
{
    NodeToTaskMapping solution;
    auto tasksSortedByShortestRemainingTime = tasks;

    auto &nodes = infrastructure->getNodes();
    std::map<NodeId, std::uint32_t> nodesRemainingTimes;
    for (auto &&node : nodes)
    {
        const auto task = node->getTask();
        nodesRemainingTimes[node->getId()] = 0;
        if (task != nullptr)
        {
            solution[node->getId()].push_back(task);
            nodesRemainingTimes[node->getId()] += task->estimateTimeLeft();
        }
    }

    std::sort(tasksSortedByShortestRemainingTime.begin(), tasksSortedByShortestRemainingTime.end(),
              [](auto &&lhs, auto &&rhs) { return lhs->estimateTimeLeft() < rhs->estimateTimeLeft(); });

    for (auto i = 0u; i < tasksSortedByShortestRemainingTime.size(); ++i)
    {
        NodePtrVec feasibleNodes;
        std::copy_if(nodes.cbegin(), nodes.cend(), std::back_inserter(feasibleNodes),
                     [task = tasksSortedByShortestRemainingTime[i]](auto &&node) { return node->canTaskFit(task); });

        const auto node = std::min_element(
            feasibleNodes.begin(), feasibleNodes.end(), [&nodesRemainingTimes](auto &&leftNode, auto &&rightNode) {
                return nodesRemainingTimes[leftNode->getId()] < nodesRemainingTimes[rightNode->getId()];
            });

        solution[(*node)->getId()].push_back(tasksSortedByShortestRemainingTime[i]);
        nodesRemainingTimes[(*node)->getId()] += tasksSortedByShortestRemainingTime[i]->estimateTimeLeft();
    }

    return solution;
}

std::string ShortestRemainingTimeFirst::toString() const
{
    return "ShortestRemainingTimeFirst";
}

} // namespace shortestremainingtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
