#pragma once

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Policy/HeuristicPolicyWithMigrationsAndPreemptions.hpp"
#include "Cloud/Task.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace shortestremainingtimefirst
{

class ShortestRemainingTimeFirstWithMigrationsAndPreemptions : public HeuristicPolicyWithMigrationsAndPreemptions
{
  public:
    ShortestRemainingTimeFirstWithMigrationsAndPreemptions(const InfrastructureCPtr &infrastructure,
                                                           const logger::LoggerPtr &logger,
                                                           const bool withMigrationsFixing);

    std::string toString() const override;

  private:
    bool heuristic(const TaskPtr &left, const TaskPtr &right) const override;
};

} // namespace shortestremainingtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
