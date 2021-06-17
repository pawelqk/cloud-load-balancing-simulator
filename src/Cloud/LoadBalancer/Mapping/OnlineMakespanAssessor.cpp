#include "OnlineMakespanAssessor.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{

OnlineMakespanAssessor::OnlineMakespanAssessor(const InfrastructureCPtr &infrastructure)
    : infrastructure(infrastructure)
{
}

double OnlineMakespanAssessor::assess(const policy::NodeToTaskMapping &mapping)
{
    std::uint32_t makespan{0};

    auto &nodes = infrastructure->getNodes();
    for (auto &&node : nodes)
    {
        if (!mapping.contains(node->getId()))
            continue;

        std::uint32_t nodeMakespan = 0;
        const auto task = node->getTask();
        if (task != nullptr)
            nodeMakespan = task->estimateTimeLeft();

        const auto &tasks = mapping.at(node->getId());
        for (auto &&task : tasks)
        {
            const auto timeLeftForTask = task->estimateTimeLeft();
            nodeMakespan += timeLeftForTask;
        }

        if (nodeMakespan > makespan)
            makespan = nodeMakespan;
    }

    return makespan;
}

} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
