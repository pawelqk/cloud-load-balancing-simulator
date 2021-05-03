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

class HeuristicPolicyWithMigrationsAndPreemptions : public PolicyBase
{
  public:
    HeuristicPolicyWithMigrationsAndPreemptions(const InfrastructureCPtr &infrastructure,
                                                const logger::LoggerPtr &logger, const bool withMigrationsFixing);

    NodeToTaskMapping buildNodeToTaskMapping(const TaskPtrVec &tasks) override;

  protected:
    virtual bool heuristic(const TaskPtr &left, const TaskPtr &right) const = 0;
    NodeToTaskMapping build(const TaskPtrVec &tasks);

  private:
    NodeToTaskMapping buildWithMigrationsFixing(const TaskPtrVec &tasks);
    bool validateSolution(const TaskPtrVec &insertedTasks, const NodeToTaskMapping &mapping);

    bool withMigrationsFixing;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
