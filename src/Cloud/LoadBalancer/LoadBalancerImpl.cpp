#include "LoadBalancerImpl.hpp"
#include <iostream>
#include <stdexcept>

namespace cloud
{
namespace loadbalancer
{

LoadBalancerImpl::LoadBalancerImpl(const NodeVec &nodes, strategy::StrategyPtr &&strategy)
    : nodes(nodes), strategy(std::move(strategy))
{
}

void LoadBalancerImpl::schedule(const TaskSet &tasks)
{
    TaskSet tasksToSchedule;
    std::set_union(tasks.cbegin(), tasks.cend(), waitingTasks.cbegin(), waitingTasks.cend(),
                   std::inserter(tasksToSchedule, tasksToSchedule.cend()));

    const auto mapping = strategy->buildTaskToNodeMapping(tasksToSchedule, nodes);
    for (auto &&taskToNode : mapping)
    {
        if (taskToNode.second.has_value())
        {
            const auto nodeIt = std::find(nodes.begin(), nodes.end(), taskToNode.second.value());
            if (nodeIt != nodes.end())
            {
                nodeIt->assign(taskToNode.first);
                const auto waitingTaskIt = std::find(waitingTasks.begin(), waitingTasks.end(), taskToNode.first);
                if (waitingTaskIt != waitingTasks.end())
                    waitingTasks.erase(waitingTaskIt);
            }
            else
                throw std::runtime_error("Node given by strategy should be present in load balancer");
        }
        else
        {
            std::cout << "Task has no node assigned to it. Putting into queue\n";
            waitingTasks.insert(taskToNode.first);
        }
    }
}

void LoadBalancerImpl::tick()
{
    for (auto &&node : nodes)
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

void LoadBalancerImpl::scheduleWaitingTasks()
{
    // TODO: maybe reuse schedule method
    for (auto taskIt = waitingTasks.begin(); taskIt != waitingTasks.end();)
    {
        const auto nodeIt = std::find_if(nodes.begin(), nodes.end(),
                                         [taskIt](auto &&node) { return node.isIdle() && node.canTaskFit(*taskIt); });

        if (nodeIt != nodes.cend())
        {
            nodeIt->assign(*taskIt);
            taskIt = waitingTasks.erase(taskIt);
        }
        else
            ++taskIt;
    }
}

bool LoadBalancerImpl::areNodesIdle() const
{
    return std::all_of(nodes.cbegin(), nodes.cend(), [](auto &&node) { return node.isIdle(); });
}

} // namespace loadbalancer
} // namespace cloud
