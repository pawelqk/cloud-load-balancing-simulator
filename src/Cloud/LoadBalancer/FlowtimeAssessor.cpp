#include "FlowtimeAssessor.hpp"

namespace cloud
{
namespace loadbalancer
{

FlowtimeAssessor::FlowtimeAssessor(const InfrastructureCPtr &infrastructure) : SolutionAssessorBase(infrastructure)
{
}

double FlowtimeAssessor::assess(const policy::Solution &solution)
{
    std::uint32_t flowtime{0};

    const auto &nodes = infrastructure->getNodes();

    for (auto &&[nodeId, tasks] : solution)
    {
        const auto nodeIt = std::find_if(nodes.begin(), nodes.end(),
                                         [nodeId = nodeId](auto &&node) { return node->getId() == nodeId; });
        if (nodeIt == nodes.end())
            throw std::runtime_error("Cannot find node " + std::to_string(nodeId) + " in infrastructure");

        std::uint32_t nodeFlowtime;
        const auto task = (*nodeIt)->getTask();
        if (task != std::nullopt)
            nodeFlowtime = task->estimateTimeLeft();
        else
            nodeFlowtime = 0;

        for (auto &&task : tasks)
        {
            nodeFlowtime += (nodeFlowtime + task.estimateTimeLeft());
        }

        flowtime += nodeFlowtime;
    }

    return flowtime;
}

} // namespace loadbalancer
} // namespace cloud
