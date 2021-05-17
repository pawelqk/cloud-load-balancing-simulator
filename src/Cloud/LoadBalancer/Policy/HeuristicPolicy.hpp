#pragma once

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"
#include "Cloud/Task.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

class HeuristicPolicy : public PolicyBase
{
  public:
    HeuristicPolicy(const InfrastructureCPtr &infrastructure, const logger::LoggerPtr &logger);

  protected:
    virtual bool heuristic(const TaskPtr &left, const TaskPtr &right) const = 0;

  private:
    NodeToTaskMapping buildNodeToTaskMappingInternal(const TaskPtrVec &tasks) override;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
