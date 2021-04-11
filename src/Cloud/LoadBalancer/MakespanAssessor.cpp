#include "MakespanAssessor.hpp"

namespace cloud
{
namespace loadbalancer
{

MakespanAssessor::MakespanAssessor(const InfrastructureCPtr &infrastructure) : SolutionAssessorBase(infrastructure)
{
}

double MakespanAssessor::assess(const policy::Solution &solution)
{
    std::uint32_t makespan{0};

    const auto &nodes = infrastructure->getNodes();

    for (auto &&[nodeId, tasks] : solution)
    {
        const auto nodeIt = std::find_if(nodes.begin(), nodes.end(),
                                         [nodeId = nodeId](auto &&node) { return node->getId() == nodeId; });
        if (nodeIt == nodes.end())
            throw std::runtime_error("Cannot find node " + std::to_string(nodeId) + " in infrastructure");

        std::uint32_t nodeMakespan;
        const auto task = (*nodeIt)->getTask();
        if (task != std::nullopt)
            nodeMakespan = task->estimateTimeLeft();
        else
            nodeMakespan = 0;

        for (auto &&task : tasks)
        {
            nodeMakespan += task.estimateTimeLeft();
        }

        if (nodeMakespan > makespan)
            makespan = nodeMakespan;
    }

    return makespan;
}

} // namespace loadbalancer
} // namespace cloud
