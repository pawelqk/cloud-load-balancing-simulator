#pragma once

#include <cstdint>
#include <vector>

#include "Infrastructure.hpp"
#include "Configuration/Instance.hpp"
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
          const TimingServicePtr &timingService, const logger::LoggerPtr &logger);

    void tick(const configuration::TaskDataVec taskDatas);

    bool isIdle() const;

    std::string toString() const;

  private:
    TaskPtrVec createTasks(const configuration::TaskDataVec taskDatas);
    void addFlowtime(const TaskPtrVec &finishedTasks);
    void logNewTasks(const TaskPtrVec &tasks);

    const loadbalancer::LoadBalancerPtr loadBalancer;
    const InfrastructurePtr infrastructure;
    const TimingServicePtr timingService;
    const logger::LoggerPtr logger;

    std::uint32_t nextTaskId;
};

} // namespace cloud
