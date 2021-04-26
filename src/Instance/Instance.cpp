#include "Instance.hpp"
#include <sstream>

namespace instance
{

Instance::Instance(const std::uint32_t id, const std::map<std::uint32_t, TaskDataVec> &tasks,
                   const std::vector<std::uint32_t> &nodesMips)
    : id(id), tasks(tasks), nodesMips(nodesMips)
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

std::string Instance::toString() const
{
    std::stringstream ss;
    ss << "Nodes : [";
    for (auto i = 0u; i < nodesMips.size() - 1; ++i)
        ss << nodesMips[i] << ", ";
    ss << nodesMips[nodesMips.size() - 1];
    ss << "]\n";

    ss << "Tasks:\n";
    for (auto &&[time, tasksInTime] : tasks)
    {
        ss << "T=" << time << ": [";
        for (auto i = 0u; i < tasksInTime.size() - 1; ++i)
            ss << "(mips: " << tasksInTime[i].requiredMips << ", length: " << tasksInTime[i].length << "), ";
        ss << "(mips: " << tasksInTime[tasksInTime.size() - 1].requiredMips
           << ", length: " << tasksInTime[tasksInTime.size() - 1].length << ")";
        ss << "]\n";
    }

    auto output = ss.str();

    const auto newLineToDeletePos = output.rfind('\n');
    if (newLineToDeletePos != std::string::npos)
        output.erase(newLineToDeletePos);

    return output;
}

} // namespace instance
