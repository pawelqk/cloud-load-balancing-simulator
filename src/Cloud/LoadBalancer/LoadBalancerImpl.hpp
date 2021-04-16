#pragma once

#include "Cloud/Infrastructure.hpp"
#include "LoadBalancer.hpp"
#include "Logger/Logger.hpp"
#include "Policy/Policy.hpp"

namespace cloud
{
namespace loadbalancer
{

using Solution = std::map<NodeId, std::list<Task>>;

class LoadBalancerImpl : public LoadBalancer
{
  public:
    LoadBalancerImpl(policy::PolicyPtr &&policy, const InfrastructurePtr &infrastructure,
                     const logger::LoggerPtr &logger);

    void scheduleNewTasks(const TaskSet &tasks) override;
    void scheduleWaitingTasks() override;

    bool areAnyTasksWaiting() const override;

  private:
    TaskSet getWaitingTasks();
    std::vector<NodeId> extractFreeNodeIds();

    const policy::PolicyPtr policy;
    const InfrastructurePtr infrastructure;

    policy::Solution solution;
    const logger::LoggerPtr logger;
};

} // namespace loadbalancer
} // namespace cloud
