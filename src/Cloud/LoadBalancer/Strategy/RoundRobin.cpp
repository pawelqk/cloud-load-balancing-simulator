#include "RoundRobin.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace strategy
{

std::map<Task, std::optional<Node>> RoundRobin::buildTaskToNodeMapping(const TaskSet &tasks, const NodeVec &nodes)
{
    std::map<Task, std::optional<Node>> mapping;
    NodeSet busyNodes;

    for (auto &&task : tasks)
    {
        const auto nodeIt = std::find_if(nodes.begin(), nodes.end(), [&task, &busyNodes](auto &&node) {
            return !busyNodes.contains(node) && node.isIdle() && node.canTaskFit(task);
        });

        if (nodeIt != nodes.end())
        {
            mapping.emplace(task, *nodeIt);
            busyNodes.insert(*nodeIt);
        }
        else
            mapping.emplace(task, std::nullopt);
    }

    return mapping;
}

} // namespace strategy
} // namespace loadbalancer
} // namespace cloud
