#pragma once

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Policy/HeuristicPolicy.hpp"
#include "Cloud/Task.hpp"
#include "Logger/Logger.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace firstcomefirstserve
{

class FirstComeFirstServe : public HeuristicPolicy
{
  public:
    FirstComeFirstServe(const InfrastructureCPtr &infrastructure, const logger::LoggerPtr &logger);

    std::string toString() const override;

  private:
    bool heuristic(const TaskPtr &left, const TaskPtr &right) const override;
};

} // namespace firstcomefirstserve
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
