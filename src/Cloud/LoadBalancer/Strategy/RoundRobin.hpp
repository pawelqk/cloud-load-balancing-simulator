#pragma once

#include <map>
#include <memory>
#include <optional>
#include <vector>

#include "Cloud/Node.hpp"
#include "Cloud/Task.hpp"
#include "Strategy.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace strategy
{

class RoundRobin : public Strategy
{
  public:
    std::map<Task, std::optional<Node>> buildTaskToNodeMapping(const TaskSet &tasks, const NodeVec &nodes) override;
};

} // namespace strategy
} // namespace loadbalancer
} // namespace cloud
