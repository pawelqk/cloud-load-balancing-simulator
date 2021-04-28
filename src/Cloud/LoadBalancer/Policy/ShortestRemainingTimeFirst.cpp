#include "ShortestRemainingTimeFirst.hpp"

#include <algorithm>
#include <iterator>

namespace cloud
{
namespace loadbalancer
{
namespace policy
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
    std::map<TaskPtr, NodeId> currentMapping;
    for (auto &&node : nodes)
    {
        const auto task = node->getTask();
        if (task != nullptr)
        {
            solution[node->getId()].push_back(task);
            tasksSortedByShortestRemainingTime.push_back(task);
            currentMapping[task] = node->getId();
        }
    }

    std::sort(tasksSortedByShortestRemainingTime.begin(), tasksSortedByShortestRemainingTime.end(),
              [](auto &&lhs, auto &&rhs) { return lhs->estimateTimeLeft() < rhs->estimateTimeLeft(); });

    const auto getNodeRemainingTime = [&solution](auto &&nodeId) {
        auto remainingTime = 0u;
        for (auto &&task : solution[nodeId])
            remainingTime += task->estimateTimeLeft();

        return remainingTime;
    };

    for (auto i = 0u; i < nodes.size() && i < tasksSortedByShortestRemainingTime.size(); ++i)
    {
        if (currentMapping.contains(tasksSortedByShortestRemainingTime[i]))
            solution[nodes[i]->getId()].push_back(tasksSortedByShortestRemainingTime[i]);
    }

    for (auto i = nodes.size(); i < tasksSortedByShortestRemainingTime.size(); ++i)
    {
        NodePtrVec feasibleNodes;
        std::copy_if(nodes.cbegin(), nodes.cend(), std::back_inserter(feasibleNodes),
                     [task = tasksSortedByShortestRemainingTime[i]](auto &&node) { return node->canTaskFit(task); });

        const auto node = std::min_element(
            feasibleNodes.begin(), feasibleNodes.end(), [getNodeRemainingTime](auto &&leftNode, auto &&rightNode) {
                return getNodeRemainingTime(leftNode->getId()) < getNodeRemainingTime(rightNode->getId());
            });

        solution[(*node)->getId()].push_back(tasksSortedByShortestRemainingTime[i]);
    }

    return solution;
}

std::string ShortestRemainingTimeFirst::toString() const
{
    return "ShortestRemainingTimeFirst";
}

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
