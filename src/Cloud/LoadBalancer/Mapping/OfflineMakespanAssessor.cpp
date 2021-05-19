#include "OfflineMakespanAssessor.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{

double OfflineMakespanAssessor::assess(const policy::NodeToTaskMapping &mapping)
{
    std::uint32_t makespan{0};
    for (auto &&[nodeId, tasks] : mapping)
    {
        std::uint32_t nodeMakespan{0};

        for (auto &&task : tasks)
        {
            const auto timeLeftForTask = task->estimateTimeLeft();
            const auto taskArrivalTime = task->getArrivalTime();

            std::uint32_t timeBetweenLastTaskFinishAndNewTaskArrival{0};
            if (nodeMakespan < taskArrivalTime)
                timeBetweenLastTaskFinishAndNewTaskArrival = taskArrivalTime - nodeMakespan;

            nodeMakespan += timeLeftForTask + timeBetweenLastTaskFinishAndNewTaskArrival;
        }

        if (nodeMakespan > makespan)
            makespan = nodeMakespan;
    }

    return makespan;
}

} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
