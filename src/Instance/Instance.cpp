#include "Instance.hpp"

namespace instance
{

Instance::Instance(const std::map<std::uint32_t, TaskDataVec> &tasks, const std::vector<std::uint32_t> &nodesMips)
    : tasks(tasks), nodesMips(nodesMips)
{
}

TaskDataVec Instance::getTasksInTimePoint(const std::uint32_t timePoint) const
{
    const auto taskIt = tasks.find(timePoint);
    if (taskIt != tasks.end())
    {
        const auto tasksInTimePoint = taskIt->second;
        return tasksInTimePoint;
    }

    return {};
}

const std::vector<std::uint32_t> &Instance::getNodesMips() const
{
    return nodesMips;
}

bool Instance::allTasksInserted(const std::uint32_t currentPointInTime) const
{
    const auto lastScheduledPointInTime = tasks.rbegin()->first;
    return currentPointInTime >= lastScheduledPointInTime;
}

} // namespace instance
