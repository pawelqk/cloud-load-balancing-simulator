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

struct Migration
{
    NodeId source;
    std::optional<NodeId> destination;
};

struct MappingActions
{
    std::map<Task, std::optional<NodeId>> assignments;
    std::map<Task, Migration> migrations;
};

class Policy
{
  public:
    Policy(const InfrastructureCPtr &infrastructure);
    virtual ~Policy() = default;

    virtual MappingActions buildTaskToNodeMapping(const TaskSet &tasks) = 0;

  protected:
    const InfrastructureCPtr infrastructure;
};

using PolicyPtr = std::unique_ptr<Policy>;

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
