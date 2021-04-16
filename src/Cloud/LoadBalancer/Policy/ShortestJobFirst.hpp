#pragma once

#include "Cloud/Infrastructure.hpp"
#include "Cloud/Task.hpp"
#include "PolicyBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

class ShortestJobFirst : public PolicyBase
{
  public:
    ShortestJobFirst(const InfrastructureCPtr &infrastructure);

    MappingActions buildTaskToNodeMapping(const TaskSet &tasks) override;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
