#include "LoadBalancerImpl.hpp"
#include <iostream>
#include <stdexcept>

namespace cloud
{
namespace loadbalancer
{

LoadBalancerImpl::LoadBalancerImpl(policy::PolicyPtr &&policy, const InfrastructurePtr &infrastructure)
    : policy(std::move(policy)), infrastructure(infrastructure), logger("LoadBalancer")
{
}

void LoadBalancerImpl::scheduleNewTasks(const TaskSet &tasks)
{
    logger.log("Scheduling %u new tasks", tasks.size());

    TaskSet tasksToSchedule;
    const auto waitingTasks = getWaitingTasks();
    std::set_union(tasks.cbegin(), tasks.cend(), waitingTasks.cbegin(), waitingTasks.cend(),
                   std::inserter(tasksToSchedule, tasksToSchedule.cend()));

    const auto mapping = policy->buildTaskToNodeMapping(tasksToSchedule);
    auto &nodes = infrastructure->getNodes();

    for (auto &&taskToMigration : mapping.migrations)
    {
        const auto sourceNodeIt =
            std::find_if(nodes.begin(), nodes.end(),
                         [nodeId = taskToMigration.second.source](auto &&node) { return node->getId() == nodeId; });
        if (sourceNodeIt != nodes.end())
        {
            logger.log("Extracting %s from %s", taskToMigration.first.toString().c_str(),
                       (*sourceNodeIt)->toString().c_str());
            (*sourceNodeIt)->extractTask();
        }
        else
            throw std::runtime_error("Node given by policy should be present in load balancer");

        if (taskToMigration.second.destination.has_value())
        {
            const auto destinationNodeIt =
                std::find_if(nodes.begin(), nodes.end(), [nodeId = *taskToMigration.second.destination](auto &&node) {
                    return node->getId() == nodeId;
                });
            if (destinationNodeIt != nodes.end())
            {
                logger.log("Migrating %s to %s", taskToMigration.first.toString().c_str(),
                           (*destinationNodeIt)->toString().c_str());
                (*destinationNodeIt)->assign(taskToMigration.first);
            }
            else
                throw std::runtime_error("Node given by policy should be present in load balancer");
        }
        else
        {
            logger.log("Migrating %s back to waiting queue", taskToMigration.first.toString().c_str());
        }
    }

    solution = std::move(mapping.solution);

    scheduleWaitingTasks();
}

void LoadBalancerImpl::scheduleWaitingTasks()
{
    auto &nodes = infrastructure->getNodes();
    for (auto &&nodeId : extractFreeNodeIds())
    {
        if (solution[nodeId].empty())
            continue;

        const auto task = solution[nodeId].front();
        const auto nodeIt = std::find_if(nodes.begin(), nodes.end(),
                                         [nodeId = nodeId](auto &&node) { return node->getId() == nodeId; });
        if (nodeIt != nodes.end())
        {
            logger.log("Assigning %s to %s", task.toString().c_str(), (*nodeIt)->toString().c_str());
            (*nodeIt)->assign(task);
            solution[nodeId].pop_front();
        }
        else
            throw std::runtime_error("Node given by policy should be present in load balancer");
    }
}

bool LoadBalancerImpl::areAnyTasksWaiting() const
{
    return std::any_of(solution.cbegin(), solution.cend(), [](auto &&entry) { return !entry.second.empty(); });
}

TaskSet LoadBalancerImpl::getWaitingTasks()
{
    TaskSet waitingTasks;
    for (auto &&entry : solution)
        waitingTasks.insert(entry.second.cbegin(), entry.second.cend());

    return waitingTasks;
}

std::vector<NodeId> LoadBalancerImpl::extractFreeNodeIds()
{
    std::vector<NodeId> freeNodeIds;
    for (auto &&node : infrastructure->getNodes())
    {
        if (node->isIdle())
            freeNodeIds.push_back(node->getId());
    }

    return freeNodeIds;
}

} // namespace loadbalancer
} // namespace cloud
