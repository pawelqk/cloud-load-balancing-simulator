#include "RoundRobin.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace strategy
{

RoundRobin::RoundRobin(const InfrastructureCPtr &infrastructure) : infrastructure(infrastructure), lastNodeIndex(0)
{
}

std::map<Task, std::optional<Node>> RoundRobin::buildTaskToNodeMapping(const TaskSet &tasks)
{
    std::map<Task, std::optional<Node>> mapping;
    NodeSet busyNodes;

    const auto &nodes = infrastructure->getNodes();
    for (auto &&task : tasks)
    {
        for (auto i = 0u; i < nodes.size(); ++i)
        {
            const auto node = nodes[lastNodeIndex];
            lastNodeIndex = (lastNodeIndex + 1) % nodes.size();

            if (!busyNodes.contains(node) && node.isIdle() && node.canTaskFit(task))
            {
                mapping.emplace(task, node);
                busyNodes.insert(node);
                break;
            }
        }

        if (!mapping.contains(task))
            mapping.emplace(task, std::nullopt);
    }

    return mapping;
}

} // namespace strategy
} // namespace loadbalancer
} // namespace cloud
