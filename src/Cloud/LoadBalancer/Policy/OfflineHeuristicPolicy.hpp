#pragma once

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"
#include "Cloud/Task.hpp"
#include "Configuration/Instance.hpp"
#include "HeuristicPolicyWithMigrationsAndPreemptions.hpp"
#include "OfflineProblemAdapter.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

class OfflineHeuristicPolicy : public HeuristicPolicyWithMigrationsAndPreemptions
{
  public:
    OfflineHeuristicPolicy(const InfrastructurePtr &infrastructure, const logger::LoggerPtr &logger,
                           const configuration::Instance &instance);

    NodeToTaskMapping buildNodeToTaskMapping(const TaskPtrVec &tasks) override;

  private:
    NodeToTaskMapping buildSolution(const InfrastructurePtr &infrastructure, const configuration::Instance &Instance);

    OfflineProblemAdapter offlineProblemAdapter;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
