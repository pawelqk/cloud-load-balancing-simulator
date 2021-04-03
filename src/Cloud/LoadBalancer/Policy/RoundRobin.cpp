#include "RoundRobin.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

RoundRobin::RoundRobin(const InfrastructureCPtr &infrastructure) : Policy(infrastructure), lastNodeIndex(0)
{
}

MappingActions RoundRobin::buildTaskToNodeMapping(const TaskSet &tasks)
{
    std::map<Task, std::optional<NodeId>> mapping;
    std::set<NodeId> busyNodeIds;

    const auto &nodes = infrastructure->getNodes();
    for (auto &&task : tasks)
    {
        for (auto i = 0u; i < nodes.size(); ++i)
        {
            const auto &node = nodes[lastNodeIndex];
            lastNodeIndex = (lastNodeIndex + 1) % nodes.size();

            if (!busyNodeIds.contains(node.getId()) && node.isIdle() && node.canTaskFit(task))
            {
                mapping.emplace(task, node.getId());
                busyNodeIds.insert(node.getId());
                break;
            }
        }

        if (!mapping.contains(task))
            mapping.emplace(task, std::nullopt);
    }

    MappingActions actions;
    actions.assignments = std::move(mapping);
    return actions;
}

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
