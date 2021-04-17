#include "LoadBalancerImpl.hpp"
#include <iostream>
#include <stdexcept>

namespace cloud
{
namespace loadbalancer
{

LoadBalancerImpl::LoadBalancerImpl(policy::PolicyPtr &&policy, const InfrastructurePtr &infrastructure,
                                   const mapping::DifferenceCalculatorPtr &differenceCalculator,
                                   const logger::LoggerPtr &logger)
    : policy(std::move(policy)), infrastructure(infrastructure), differenceCalculator(differenceCalculator),
      logger(logger)
{
}

void LoadBalancerImpl::scheduleNewTasks(const TaskPtrVec &tasks)
{
    logger->debug("Scheduling %u new tasks", tasks.size());

    TaskPtrVec tasksToSchedule = getWaitingTasks();
    tasksToSchedule.insert(tasksToSchedule.end(), tasks.cbegin(), tasks.cend());

    nodeToTaskMapping = policy->buildNodeToTaskMapping(tasksToSchedule);

    const auto difference = differenceCalculator->calculate(nodeToTaskMapping);

    handlePreemptions(difference.preemptions);

    handleMigrations(difference.migrations);

    for (auto &&nodeId : difference.nodeIdsWithoutChange)
        nodeToTaskMapping[nodeId].pop_front();

    scheduleWaitingTasks();
}

void LoadBalancerImpl::scheduleWaitingTasks()
{
    auto &nodes = infrastructure->getNodes();
    for (auto &&nodeId : extractFreeNodeIds())
    {
        if (nodeToTaskMapping[nodeId].empty())
            continue;

        const auto task = nodeToTaskMapping[nodeId].front();
        const auto nodeIt = std::find_if(nodes.begin(), nodes.end(),
                                         [nodeId = nodeId](auto &&node) { return node->getId() == nodeId; });
        if (nodeIt != nodes.end())
        {
            logger->debug("Assigning %s to %s", task->toString().c_str(), (*nodeIt)->toString().c_str());
            (*nodeIt)->assign(task);
            nodeToTaskMapping[nodeId].pop_front();
        }
        else
            throw std::runtime_error("Node given by policy should be present in load balancer");
    }
}

bool LoadBalancerImpl::areAnyTasksWaiting() const
{
    return std::any_of(nodeToTaskMapping.cbegin(), nodeToTaskMapping.cend(),
                       [](auto &&entry) { return !entry.second.empty(); });
}

TaskPtrVec LoadBalancerImpl::getWaitingTasks()
{
    TaskPtrVec waitingTasks;
    for (auto &&entry : nodeToTaskMapping)
        waitingTasks.insert(waitingTasks.end(), entry.second.cbegin(), entry.second.cend());

    return waitingTasks;
}

void LoadBalancerImpl::handlePreemptions(const std::vector<mapping::Preemption> &preemptions)
{
    auto &nodes = infrastructure->getNodes();
    for (auto &&preemption : preemptions)
    {
        const auto sourceNodeIt = std::find_if(
            nodes.begin(), nodes.end(), [nodeId = preemption.source](auto &&node) { return node->getId() == nodeId; });
        if (sourceNodeIt != nodes.end())
        {
            logger->debug("Preempting %s from %s", preemption.task->toString().c_str(),
                          (*sourceNodeIt)->toString().c_str());
            (*sourceNodeIt)->extractTask()->performPreemption();
        }
        else
            throw std::runtime_error("Node given by policy should be present in load balancer");
    }
}

void LoadBalancerImpl::handleMigrations(const std::vector<mapping::Migration> &migrations)
{
    auto &nodes = infrastructure->getNodes();
    for (auto &&migration : migrations)
    {
        const auto sourceNodeIt = std::find_if(
            nodes.begin(), nodes.end(), [nodeId = migration.source](auto &&node) { return node->getId() == nodeId; });
        if (sourceNodeIt != nodes.end())
        {
            logger->debug("Extracting %s from %s", migration.task->toString().c_str(),
                          (*sourceNodeIt)->toString().c_str());
            (*sourceNodeIt)->extractTask()->performMigration();
        }
        else
            throw std::runtime_error("Node given by policy should be present in load balancer");
    }
    for (auto &&migration : migrations)
    {
        const auto destinationNodeIt =
            std::find_if(nodes.begin(), nodes.end(),
                         [nodeId = migration.destination](auto &&node) { return node->getId() == nodeId; });
        if (destinationNodeIt != nodes.end())
        {
            logger->debug("Migrating %s to %s", migration.task->toString().c_str(),
                          (*destinationNodeIt)->toString().c_str());
            (*destinationNodeIt)->assign(migration.task);
            nodeToTaskMapping[(*destinationNodeIt)->getId()].pop_front();
        }
        else
            throw std::runtime_error("Node given by policy should be present in load balancer");
    }
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
