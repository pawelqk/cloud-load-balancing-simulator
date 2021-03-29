#pragma once

#include <map>
#include <memory>
#include <optional>
#include <vector>

#include "Cloud/Node.hpp"
#include "Cloud/Task.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace strategy
{

class Strategy
{
  public:
    virtual ~Strategy() = default;

    virtual std::map<Task, std::optional<Node>> buildTaskToNodeMapping(const TaskSet &tasks, const NodeVec &nodes) = 0;
};

using StrategyPtr = std::unique_ptr<Strategy>;

} // namespace strategy
} // namespace loadbalancer
} // namespace cloud
