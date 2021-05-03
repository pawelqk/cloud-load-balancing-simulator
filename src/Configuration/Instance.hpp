#pragma once

#include <algorithm>
#include <cstdint>
#include <map>
#include <vector>

#include "Cloud/Node.hpp"
#include "Cloud/Task.hpp"

namespace configuration
{

struct TaskData
{
    std::uint32_t requiredMips;
    std::uint32_t length;
};

using TaskDataVec = std::vector<TaskData>;

class Instance
{
  public:
    Instance(const std::uint32_t id, const std::map<std::uint32_t, TaskDataVec> &tasks,
             const std::vector<std::uint32_t> &nodesMips);

    std::uint32_t getId() const;

    TaskDataVec getTasksInTimePoint(const std::uint32_t timePoint) const;

    const std::vector<std::uint32_t> &getNodesMips() const;

    bool allTasksInserted(const std::uint32_t currentPointInTime) const;

    std::string toString() const;

  private:
    const std::uint32_t id;
    const std::map<std::uint32_t, TaskDataVec> tasks;
    const std::vector<std::uint32_t> nodesMips;
};

} // namespace configuration
