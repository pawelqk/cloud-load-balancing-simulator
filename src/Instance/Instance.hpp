#pragma once

#include <algorithm>
#include <cstdint>
#include <map>
#include <vector>

#include "Cloud/Node.hpp"
#include "Cloud/Task.hpp"

namespace instance
{

class Instance
{
  public:
    Instance(const std::map<std::uint32_t, std::vector<cloud::Task>> &tasks,
             const std::vector<std::uint32_t> &nodesMips)
        : tasks(tasks), nodesMips(nodesMips)
    {
    }

    std::vector<cloud::Task> getTasksInTimePoint(const std::uint32_t timePoint)
    {
        const auto taskIt = tasks.find(timePoint);
        if (taskIt != tasks.end())
        {
            const auto tasksInTimePoint = taskIt->second;
            return tasksInTimePoint;
        }

        return {};
    }

    const std::vector<std::uint32_t> &getNodesMips() const
    {
        return nodesMips;
    }

    bool allTasksInserted(const std::uint32_t currentPointInTime)
    {
        const auto lastScheduledPointInTime = tasks.rbegin()->first;
        return currentPointInTime >= lastScheduledPointInTime;
    }

  private:
    std::map<std::uint32_t, std::vector<cloud::Task>> tasks;
    std::vector<std::uint32_t> nodesMips;
};

} // namespace instance
