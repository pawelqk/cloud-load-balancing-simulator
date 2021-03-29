#pragma once

#include "LoadBalancer.hpp"
#include "Strategy/Strategy.hpp"

namespace cloud
{
namespace loadbalancer
{

class LoadBalancerImpl : public LoadBalancer
{
  public:
    LoadBalancerImpl(const NodeVec &nodes, strategy::StrategyPtr &&strategy);

    void schedule(const TaskSet &tasks) override;
    void tick() override;
    bool isIdle() const override;

  private:
    bool areNodesIdle() const;
    void scheduleWaitingTasks();

    const strategy::StrategyPtr strategy;

    NodeVec nodes;
    TaskSet waitingTasks;
};

} // namespace loadbalancer
} // namespace cloud
