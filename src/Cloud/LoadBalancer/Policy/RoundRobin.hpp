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

class RoundRobin : public PolicyBase
{
  public:
    RoundRobin(const InfrastructureCPtr &infrastructure);

    NodeToTaskMapping buildNodeToTaskMapping(const TaskPtrVec &tasks) override;

  private:
    NodePtrVec::size_type lastNodeIndex;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
