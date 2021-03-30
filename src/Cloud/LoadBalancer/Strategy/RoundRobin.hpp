#pragma once

#include <map>
#include <memory>
#include <optional>
#include <vector>

#include "Cloud/Infrastructure.hpp"
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
    RoundRobin(const InfrastructureCPtr &infrastructure);

    std::map<Task, std::optional<Node>> buildTaskToNodeMapping(const TaskSet &tasks) override;

  private:
    const InfrastructureCPtr infrastructure;

    NodeVec::size_type lastNodeIndex;
};

} // namespace strategy
} // namespace loadbalancer
} // namespace cloud
