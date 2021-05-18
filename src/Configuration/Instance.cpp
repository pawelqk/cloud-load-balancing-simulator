#include "Instance.hpp"
#include <sstream>

namespace configuration
{

Instance::Instance(const std::uint32_t id, const std::map<std::uint32_t, TaskDataVec> &tasks, const NodeDataVec &nodes)
    : id(id), tasks(tasks), nodes(nodes)
{
}

std::uint32_t Instance::getId() const
{
    return id;
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

const std::map<std::uint32_t, TaskDataVec> &Instance::getTasks() const
{
    return tasks;
}

const NodeDataVec &Instance::getNodesData() const
{
    return nodes;
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
    for (auto i = 0u; i < nodes.size() - 1; ++i)
        ss << "(id: " << nodes[i].id << ", mips: " << nodes[i].mips << "), ";
    ss << "(id: " << nodes[nodes.size() - 1].id << ", mips: " << nodes[nodes.size() - 1].mips << ")]\n";

    ss << "Tasks:\n";
    for (auto &&[time, tasksInTime] : tasks)
    {
        ss << "T=" << time << ": [";
        for (auto i = 0u; i < tasksInTime.size() - 1; ++i)
            ss << "(id: " << tasksInTime[i].id << ", mips: " << tasksInTime[i].requiredMips
               << ", length: " << tasksInTime[i].length << "), ";
        ss << "(id: " << tasksInTime[tasksInTime.size() - 1].id
           << ", mips: " << tasksInTime[tasksInTime.size() - 1].requiredMips
           << ", length: " << tasksInTime[tasksInTime.size() - 1].length << ")";
        ss << "]\n";
    }

    auto output = ss.str();

    return output;
}

} // namespace configuration
