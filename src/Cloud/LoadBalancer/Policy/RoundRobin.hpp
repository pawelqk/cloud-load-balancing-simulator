#pragma once

#include <map>
#include <memory>
#include <optional>
#include <vector>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/Node.hpp"
#include "Cloud/Task.hpp"
#include "Policy.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

class RoundRobin : public Policy
{
  public:
    RoundRobin(const InfrastructureCPtr &infrastructure);

    MappingActions buildTaskToNodeMapping(const TaskSet &tasks) override;

  private:
    NodeVec::size_type lastNodeIndex;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
