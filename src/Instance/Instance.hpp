#pragma once

#include <algorithm>
#include <vector>

#include "Cloud/Node.hpp"
#include "Cloud/Task.hpp"

namespace instance
{

class Instance
{
  public:
    Instance(const std::vector<cloud::Task> &tasks, const std::vector<cloud::Node> &nodes);

    const std::vector<cloud::Task> &getTasks() const
    {
        return tasks;
    }

    const std::vector<cloud::Node> &getNodes() const
    {
        return nodes;
    }

  private:
    std::vector<cloud::Task> tasks;
    std::vector<cloud::Node> nodes;
};

} // namespace instance
