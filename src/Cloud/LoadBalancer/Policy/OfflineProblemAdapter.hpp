#pragma once

#include "Cloud/Task.hpp"
#include "Policy.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

class OfflineProblemAdapter
{
  public:
    OfflineProblemAdapter(const InfrastructureCPtr &infrastructure, const NodeToTaskMapping &solution);

    NodeToTaskMapping getNextMapping(const TaskPtrVec &tasks);

  private:
    NodeToTaskMapping mapNextTasks(const TaskPtrVec &tasks);

    const InfrastructureCPtr infrastructure;
    const NodeToTaskMapping solution;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
