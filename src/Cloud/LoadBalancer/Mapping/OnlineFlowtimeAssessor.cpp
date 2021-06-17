#include "OnlineFlowtimeAssessor.hpp"
#include <cstdint>

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{

OnlineFlowtimeAssessor::OnlineFlowtimeAssessor(const InfrastructureCPtr &infrastructure,
                                               const TimingServicePtr &timingService)
    : infrastructure(infrastructure), timingService(timingService)
{
}

double OnlineFlowtimeAssessor::assess(const policy::NodeToTaskMapping &mapping)
{
    std::uint32_t flowtime{0};

    auto &nodes = infrastructure->getNodes();
    for (auto &&node : nodes)
    {
        if (!mapping.contains(node->getId()))
            continue;

        std::uint32_t nodeFlowtime = 0;
        const auto task = node->getTask();
        if (task != nullptr)
            nodeFlowtime = task->estimateTimeLeft();

        const auto &tasks = mapping.at(node->getId());
        for (auto &&task : tasks)
        {
            const auto timeLeftForTask = task->estimateTimeLeft();
            nodeFlowtime += (nodeFlowtime + calculateTaskFlowtime(task) + timeLeftForTask);
        }

        flowtime += nodeFlowtime;
    }

    return flowtime;
}

std::uint32_t OnlineFlowtimeAssessor::calculateTaskFlowtime(const TaskPtr &task)
{
    return timingService->getTicks() - task->getArrivalTime();
}

} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
