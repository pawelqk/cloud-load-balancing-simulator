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
    Instance(const std::map<std::uint32_t, std::vector<cloud::Task>> &tasks, const std::vector<cloud::Node> &nodes)
        : tasks(tasks), nodes(nodes)
    {
    }

    std::vector<cloud::Task> getTasksInTimePoint(const std::uint32_t timePoint)
    {
        const auto taskIt = tasks.find(timePoint);
        if (taskIt != tasks.end())
        {
            const auto tasksInTimePoint = std::move(taskIt->second);
            tasks.erase(taskIt);
            return tasksInTimePoint;
        }

        return {};
    }

    const std::vector<cloud::Node> &getNodes() const
    {
        return nodes;
    }

    bool areAnyTasksLeft()
    {
        return !tasks.empty();
    }

  private:
    std::map<std::uint32_t, std::vector<cloud::Task>> tasks;
    std::vector<cloud::Node> nodes;
};

} // namespace instance
