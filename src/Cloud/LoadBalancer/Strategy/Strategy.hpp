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
namespace strategy
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

class Strategy
{
  public:
    Strategy(const InfrastructureCPtr &infrastructure);
    virtual ~Strategy() = default;

    virtual MappingActions buildTaskToNodeMapping(const TaskSet &tasks) = 0;

  protected:
    const InfrastructureCPtr infrastructure;
};

using StrategyPtr = std::unique_ptr<Strategy>;

} // namespace strategy
} // namespace loadbalancer
} // namespace cloud
