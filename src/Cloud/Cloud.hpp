#pragma once

#include <cstdint>
#include <vector>

#include "Configuration/Instance.hpp"
#include "Infrastructure.hpp"
#include "LoadBalancer/LoadBalancer.hpp"
#include "Logger/Logger.hpp"
#include "Node.hpp"
#include "Task.hpp"
#include "TimingService.hpp"

namespace cloud
{

class Cloud
{
  public:
    Cloud(loadbalancer::LoadBalancerPtr &&loadBalancer, const InfrastructurePtr &infrastructure,
          const TimingServicePtr &timingService, const double penaltyFactor, const logger::LoggerPtr &logger);

    void tick(const configuration::TaskDataVec taskDatas);

    bool isIdle() const;

    std::string toString() const;

    bool areAllInsertedTasksFinished() const;

  private:
    TaskPtrVec createTasks(const configuration::TaskDataVec taskDatas);
    void addFlowtime(const TaskPtrVec &finishedTasks);
    void saveFinishedTasks(const TaskPtrVec &finishedTasks);
    void logNewTasks(const TaskPtrVec &tasks);

    const loadbalancer::LoadBalancerPtr loadBalancer;
    const InfrastructurePtr infrastructure;
    const TimingServicePtr timingService;
    const double penaltyFactor;
    const logger::LoggerPtr logger;

    std::set<std::uint32_t> finishedTaskIds;
    std::set<std::uint32_t> insertedTaskIds;
};

} // namespace cloud
