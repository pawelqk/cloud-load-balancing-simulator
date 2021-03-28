#include "LoadBalancerImpl.hpp"

namespace cloud
{
namespace loadbalancer
{

LoadBalancerImpl::LoadBalancerImpl(const std::vector<Node> &nodes) : nodes(nodes)
{
}

void LoadBalancerImpl::schedule(const TaskSet &tasks)
{
    // TODO: maybe introduce TaskSet
    for (auto &&task : tasks)
    {
        const auto nodeIt = std::find_if(nodes.begin(), nodes.end(),
                                         [&task](auto &&node) { return node.isIdle() && node.canTaskFit(task); });

        if (nodeIt != nodes.cend())
            nodeIt->assign(task);
        else
            waitingTasks.insert(task);
    }
}

void LoadBalancerImpl::tick()
{
    for (auto &node : nodes)
    {
        node.work();
        if (node.isIdle())
            scheduleWaitingTasks();
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
