#include "RoundRobin.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

RoundRobin::RoundRobin(const InfrastructureCPtr &infrastructure) : PolicyBase(infrastructure), lastNodeIndex(0)
{
}

MappingActions RoundRobin::buildTaskToNodeMapping(const TaskSet &tasks)
{
    MappingActions actions;

    const auto &nodes = infrastructure->getNodes();
    for (auto &&task : tasks)
    {
        for (auto i = 0u; i < nodes.size(); ++i)
        {
            const auto &node = nodes[lastNodeIndex];
            lastNodeIndex = (lastNodeIndex + 1) % nodes.size();

            if (node->canTaskFit(task))
            {
                actions.solution[node->getId()].push_back(task);
                break;
            }
        }
    }

    return actions;
}

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
