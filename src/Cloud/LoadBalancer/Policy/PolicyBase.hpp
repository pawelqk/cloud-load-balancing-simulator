#pragma once

#include "Policy.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

class PolicyBase : public Policy
{
  public:
    NodeToTaskMapping buildNodeToTaskMapping(const TaskPtrVec &tasks) override;

  protected:
    PolicyBase(const InfrastructureCPtr &infrastructure, const logger::LoggerPtr &logger);

    virtual NodeToTaskMapping buildNodeToTaskMappingInternal(const TaskPtrVec &tasks) = 0;

    const InfrastructureCPtr infrastructure;
    const logger::LoggerPtr logger;

  private:
    bool validateSolution(const TaskPtrVec &insertedTasks, const NodeToTaskMapping &mapping);
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
