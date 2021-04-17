#include "DifferenceCalculatorImpl.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{

DifferenceCalculatorImpl::DifferenceCalculatorImpl(const InfrastructureCPtr &Infrastructure)
    : Infrastructure(Infrastructure)
{
}

Difference DifferenceCalculatorImpl::calculate(const policy::NodeToTaskMapping &mapping) const
{
    const auto &nodes = Infrastructure->getNodes();

    TaskToNode currentMapping;
    for (auto &&node : nodes)
    {
        const auto task = node->getTask();
        if (task != nullptr)
            currentMapping.emplace(task, node->getId());
    }

    TaskToNode newMapping;
    for (auto &&[nodeId, tasks] : mapping)
    {
        if (!tasks.empty())
            newMapping.emplace(tasks.front(), nodeId);
    }

    return calculateDifference(currentMapping, newMapping);
}

Difference DifferenceCalculatorImpl::calculateDifference(const TaskToNode &currentMapping,
                                                         const TaskToNode &newMapping) const
{
    Difference difference;

    for (auto &&[task, currentNodeId] : currentMapping)
    {
        if (!newMapping.contains(task))
            difference.preemptions.emplace_back(currentNodeId, task);
        else if (newMapping.at(task) != currentNodeId)
            difference.migrations.emplace_back(currentNodeId, newMapping.at(task), task);
        else
            difference.nodeIdsWithoutChange.emplace_back(currentNodeId);
    }

    return difference;
}

} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
