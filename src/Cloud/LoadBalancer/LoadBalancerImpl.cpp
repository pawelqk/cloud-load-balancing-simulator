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
    for (auto &&taskToNode : mapping)
    {
        if (taskToNode.second.has_value())
        {
            const auto nodeIt = std::find(nodes.begin(), nodes.end(), taskToNode.second.value());
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

void LoadBalancerImpl::tick()
{
    for (auto &&node : infrastructure->getNodes())
    {
        if (node.isIdle())
            continue;

        node.work();
        if (node.isIdle())
            schedule({});
    }
}

bool LoadBalancerImpl::isIdle() const
{
    return areNodesIdle() && waitingTasks.empty();
}

bool LoadBalancerImpl::areNodesIdle() const
{
    const auto &nodes = infrastructure->getNodes();
    return std::all_of(nodes.cbegin(), nodes.cend(), [](auto &&node) { return node.isIdle(); });
}

} // namespace loadbalancer
} // namespace cloud
