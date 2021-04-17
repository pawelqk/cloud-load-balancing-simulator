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

NodeToTaskMapping RoundRobin::buildNodeToTaskMapping(const TaskPtrVec &tasks)
{
    const auto &nodes = infrastructure->getNodes();

    NodeToTaskMapping mapping;
    for (auto &&node : nodes)
    {
        const auto task = node->getTask();
        if (task != nullptr)
            mapping[node->getId()].push_back(task);
    }

    for (auto &&task : tasks)
    {
        for (auto i = 0u; i < nodes.size(); ++i)
        {
            const auto &node = nodes[lastNodeIndex];
            lastNodeIndex = (lastNodeIndex + 1) % nodes.size();

            if (node->canTaskFit(task))
            {
                mapping[node->getId()].push_back(task);
                break;
            }
        }
    }

    return mapping;
}

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
