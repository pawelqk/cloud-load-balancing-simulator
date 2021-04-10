#pragma once

#include "Cloud/Infrastructure.hpp"
#include "LoadBalancer.hpp"
#include "Logger/Logger.hpp"
#include "Policy/Policy.hpp"

namespace cloud
{
namespace loadbalancer
{

class LoadBalancerImpl : public LoadBalancer
{
  public:
    LoadBalancerImpl(policy::PolicyPtr &&policy, const InfrastructurePtr &infrastructure);

    void scheduleNewTasks(const TaskSet &tasks) override;
    void scheduleWaitingTasks() override;

    bool areAnyTasksWaiting() const override;

  private:
    const policy::PolicyPtr policy;
    const InfrastructurePtr infrastructure;

    TaskSet waitingTasks;
    logger::Logger logger;
};

} // namespace loadbalancer
} // namespace cloud
