#pragma once

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Policy/HeuristicPolicy.hpp"
#include "Cloud/Task.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace shortestremainingtimefirst
{

class ShortestRemainingTimeFirst : public HeuristicPolicy
{
  public:
    ShortestRemainingTimeFirst(const InfrastructureCPtr &infrastructure, const logger::LoggerPtr &logger);

    std::string toString() const override;

  private:
    bool heuristic(const TaskPtr &left, const TaskPtr &right) const override;
};

} // namespace shortestremainingtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
