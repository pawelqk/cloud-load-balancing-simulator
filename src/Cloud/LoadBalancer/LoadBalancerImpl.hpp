#pragma once

#include "Cloud/Infrastructure.hpp"
#include "LoadBalancer.hpp"
#include "Logger/Logger.hpp"
#include "Mapping/DifferenceCalculator.hpp"
#include "Policy/Policy.hpp"

namespace cloud
{
namespace loadbalancer
{

class LoadBalancerImpl : public LoadBalancer
{
  public:
    LoadBalancerImpl(policy::PolicyPtr &&policy, const InfrastructurePtr &infrastructure,
                     const mapping::DifferenceCalculatorPtr &differenceCalculator, const logger::LoggerPtr &logger);

    void scheduleNewTasks(const TaskPtrVec &tasks) override;
    void scheduleWaitingTasks() override;

    bool areAnyTasksWaiting() const override;

    std::string toString() const override;

  private:
    TaskPtrVec getWaitingTasks();
    void handlePreemptions(const std::vector<mapping::Preemption> &preemptions);
    void handleMigrations(const std::vector<mapping::Migration> &migrations);
    std::vector<NodeId> extractFreeNodeIds();

    const policy::PolicyPtr policy;
    const InfrastructurePtr infrastructure;
    const mapping::DifferenceCalculatorPtr differenceCalculator;

    policy::NodeToTaskMapping nodeToTaskMapping;
    const logger::LoggerPtr logger;
};

} // namespace loadbalancer
} // namespace cloud
