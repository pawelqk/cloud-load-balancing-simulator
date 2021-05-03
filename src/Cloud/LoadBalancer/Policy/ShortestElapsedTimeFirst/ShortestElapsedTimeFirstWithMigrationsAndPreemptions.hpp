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
namespace shortestelapsedtimefirst
{

class ShortestElapsedTimeFirstWithMigrationsAndPreemptions : public HeuristicPolicyWithMigrationsAndPreemptions
{
  public:
    ShortestElapsedTimeFirstWithMigrationsAndPreemptions(const InfrastructureCPtr &infrastructure,
                                                         const logger::LoggerPtr &logger,
                                                         const bool withMigrationsFixing);

    std::string toString() const override;

  private:
    bool heuristic(const TaskPtr &left, const TaskPtr &right) const override;
};

} // namespace shortestelapsedtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
