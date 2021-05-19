#include "OfflineFlowtimeAssessor.hpp"
#include <cstdint>

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{

double OfflineFlowtimeAssessor::assess(const policy::NodeToTaskMapping &mapping)
{
    std::uint32_t flowtime{0};

    for (auto &&[nodeId, tasks] : mapping)
    {
        std::uint32_t nodeFlowtime{0};
        std::uint32_t lastTaskFinishTime{0};
        for (auto &&task : tasks)
        {
            const auto timeLeftForTask = task->estimateTimeLeft();
            const auto taskArrivalTime = task->getArrivalTime();

            std::uint32_t timeBetweenNewTaskArrivalAndLastTaskFinish{0};
            if (lastTaskFinishTime > taskArrivalTime)
                timeBetweenNewTaskArrivalAndLastTaskFinish = lastTaskFinishTime - taskArrivalTime;

            nodeFlowtime += timeBetweenNewTaskArrivalAndLastTaskFinish + timeLeftForTask;

            std::uint32_t timeBetweenLastTaskFinishAndNewTaskArrival{0};
            if (lastTaskFinishTime < taskArrivalTime)
                timeBetweenLastTaskFinishAndNewTaskArrival = taskArrivalTime - lastTaskFinishTime;

            lastTaskFinishTime += timeLeftForTask + timeBetweenLastTaskFinishAndNewTaskArrival;
        }

        flowtime += nodeFlowtime;
    }

    return flowtime;
}

} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
