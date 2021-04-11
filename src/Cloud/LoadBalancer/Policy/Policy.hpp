#pragma once

#include <map>
#include <memory>
#include <optional>
#include <vector>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/Node.hpp"
#include "Cloud/Task.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

using Solution = std::map<NodeId, std::list<Task>>;

struct Migration
{
    NodeId source;
    std::optional<NodeId> destination;
};

struct MappingActions
{
    std::map<Task, Migration> migrations;
    Solution solution;
};

class Policy
{
  public:
    virtual ~Policy() = default;

    virtual MappingActions buildTaskToNodeMapping(const TaskSet &tasks) = 0;
};

using PolicyPtr = std::unique_ptr<Policy>;

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
