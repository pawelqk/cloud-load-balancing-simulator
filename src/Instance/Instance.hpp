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
             const std::vector<std::uint32_t> &nodesMips);

    std::vector<cloud::Task> getTasksInTimePoint(const std::uint32_t timePoint) const;

    const std::vector<std::uint32_t> &getNodesMips() const;

    bool allTasksInserted(const std::uint32_t currentPointInTime) const;

  private:
    const std::map<std::uint32_t, std::vector<cloud::Task>> tasks;
    const std::vector<std::uint32_t> nodesMips;
};

} // namespace instance
