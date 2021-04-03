#include "LoadBalancerImpl.hpp"
#include <iostream>
#include <stdexcept>

namespace cloud
{
namespace loadbalancer
{

LoadBalancerImpl::LoadBalancerImpl(strategy::StrategyPtr &&strategy, const InfrastructurePtr &infrastructure)
    : strategy(std::move(strategy)), infrastructure(infrastructure), logger("LoadBalancer")
{
}

void LoadBalancerImpl::schedule(const TaskSet &tasks)
{
    TaskSet tasksToSchedule;
    std::set_union(tasks.cbegin(), tasks.cend(), waitingTasks.cbegin(), waitingTasks.cend(),
                   std::inserter(tasksToSchedule, tasksToSchedule.cend()));

    const auto mapping = strategy->buildTaskToNodeMapping(tasksToSchedule);
    auto &nodes = infrastructure->getNodes();

    for (auto &&taskToMigration : mapping.migrations)
    {
        const auto sourceNodeIt =
            std::find_if(nodes.begin(), nodes.end(),
                         [nodeId = taskToMigration.second.source](auto &&node) { return node.getId() == nodeId; });
        if (sourceNodeIt != nodes.end())
        {
            logger.log("Extracting %s from %s", taskToMigration.first.toString().c_str(),
                       sourceNodeIt->toString().c_str());
            sourceNodeIt->extractTask();
        }
        else
            throw std::runtime_error("Node given by strategy should be present in load balancer");

        if (taskToMigration.second.destination.has_value())
        {
            const auto destinationNodeIt =
                std::find_if(nodes.begin(), nodes.end(), [nodeId = *taskToMigration.second.destination](auto &&node) {
                    return node.getId() == nodeId;
                });
            if (destinationNodeIt != nodes.end())
            {
                logger.log("Migrating %s to %s", taskToMigration.first.toString().c_str(),
                           destinationNodeIt->toString().c_str());
                destinationNodeIt->assign(taskToMigration.first);
            }
            else
                throw std::runtime_error("Node given by strategy should be present in load balancer");
        }
        else
        {
            logger.log("Migrating %s back to waiting queue", taskToMigration.first.toString().c_str());
            waitingTasks.insert(taskToMigration.first);
        }
    }

    for (auto &&taskToNode : mapping.assignments)
    {
        if (taskToNode.second.has_value())
        {
            const auto nodeIt =
                std::find_if(nodes.begin(), nodes.end(),
                             [nodeId = taskToNode.second.value()](auto &&node) { return node.getId() == nodeId; });
            if (nodeIt != nodes.end())
            {
                logger.log("Assigning %s to %s", taskToNode.first.toString().c_str(), nodeIt->toString().c_str());
                nodeIt->assign(taskToNode.first);
                const auto waitingTaskIt = std::find(waitingTasks.begin(), waitingTasks.end(), taskToNode.first);
                if (waitingTaskIt != waitingTasks.end())
                    waitingTasks.erase(waitingTaskIt);
            }
            else
                throw std::runtime_error("Node given by strategy should be present in load balancer");
        }
        else if (!waitingTasks.contains(taskToNode.first))
        {
            logger.log("%s cannot be assigned to any node. Putting into queue", taskToNode.first.toString().c_str());
            waitingTasks.insert(taskToNode.first);
        }
    }
}

bool LoadBalancerImpl::areAnyTasksWaiting() const
{
    return waitingTasks.empty();
}

} // namespace loadbalancer
} // namespace cloud
