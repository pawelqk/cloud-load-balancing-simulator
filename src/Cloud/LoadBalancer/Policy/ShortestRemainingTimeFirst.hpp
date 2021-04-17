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

class ShortestRemainingTimeFirst : public PolicyBase
{
  public:
    ShortestRemainingTimeFirst(const InfrastructureCPtr &infrastructure);

    NodeToTaskMapping buildNodeToTaskMapping(const TaskPtrVec &tasks) override;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
