#include "MakespanAssessor.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{

MakespanAssessor::MakespanAssessor(const DifferenceCalculatorPtr &differenceCalculator)
    : MappingAssessorBase(differenceCalculator)
{
}

double MakespanAssessor::assess(const policy::NodeToTaskMapping &mapping)
{
    std::uint32_t makespan{0};

    const auto difference = differenceCalculator->calculate(mapping);

    std::map<TaskPtr, std::uint32_t> timeLeftAfterChanges;
    for (auto &&preemption : difference.preemptions)
        timeLeftAfterChanges[preemption.task] = preemption.task->estimateTimeLeftAfterPreemption();

    for (auto &&migration : difference.migrations)
        timeLeftAfterChanges[migration.task] = migration.task->estimateTimeLeftAfterMigration();

    for (auto &&[nodeId, tasks] : mapping)
    {
        std::uint32_t nodeMakespan{0};

        for (auto &&task : tasks)
        {
            const auto timeLeftForTask =
                timeLeftAfterChanges.contains(task) ? timeLeftAfterChanges.at(task) : task->estimateTimeLeft();
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
