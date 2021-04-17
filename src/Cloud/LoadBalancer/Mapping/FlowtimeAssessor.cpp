#include "FlowtimeAssessor.hpp"
#include <cstdint>

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{

FlowtimeAssessor::FlowtimeAssessor(const DifferenceCalculatorPtr &differenceCalculator,
                                   const TimingServicePtr &timingService)
    : MappingAssessorBase(differenceCalculator), timingService(timingService)
{
}

double FlowtimeAssessor::assess(const policy::NodeToTaskMapping &mapping)
{
    std::uint32_t flowtime{0};

    const auto difference = differenceCalculator->calculate(mapping);

    std::map<TaskPtr, std::uint32_t> timeLeftAfterChanges;
    for (auto &&preemption : difference.preemptions)
        timeLeftAfterChanges[preemption.task] = preemption.task->estimateTimeLeftAfterPreemption();

    for (auto &&migration : difference.migrations)
        timeLeftAfterChanges[migration.task] = migration.task->estimateTimeLeftAfterMigration();

    for (auto &&[nodeId, tasks] : mapping)
    {
        std::uint32_t nodeFlowtime{0};
        for (auto &&task : tasks)
        {
            const auto timeLeftForTask =
                timeLeftAfterChanges.contains(task) ? timeLeftAfterChanges.at(task) : task->estimateTimeLeft();
            nodeFlowtime += (nodeFlowtime + calculateTaskFlowtime(task) + timeLeftForTask);
        }

        flowtime += nodeFlowtime;
    }

    return flowtime;
}

std::uint32_t FlowtimeAssessor::calculateTaskFlowtime(const TaskPtr &task)
{
    return timingService->getTicks() - task->getArrivalTime();
}

} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
