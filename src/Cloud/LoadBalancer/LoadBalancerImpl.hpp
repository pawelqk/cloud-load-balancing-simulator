#pragma once

#include "Cloud/Infrastructure.hpp"
#include "LoadBalancer.hpp"
#include "Logger/Logger.hpp"
#include "Strategy/Strategy.hpp"

namespace cloud
{
namespace loadbalancer
{

class LoadBalancerImpl : public LoadBalancer
{
  public:
    LoadBalancerImpl(strategy::StrategyPtr &&strategy, const InfrastructurePtr &infrastructure);

    void schedule(const TaskSet &tasks) override;
    void tick() override;
    bool isIdle() const override;

  private:
    bool areNodesIdle() const;

    const strategy::StrategyPtr strategy;
    const InfrastructurePtr infrastructure;

    TaskSet waitingTasks;
    logger::Logger logger;
};

} // namespace loadbalancer
} // namespace cloud
