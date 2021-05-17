#pragma once

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"
#include "Cloud/Task.hpp"
#include "Logger/Logger.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace roundrobin
{

class RoundRobin : public PolicyBase
{
  public:
    RoundRobin(const InfrastructureCPtr &infrastructure, const logger::LoggerPtr &logger);

    NodeToTaskMapping buildNodeToTaskMappingInternal(const TaskPtrVec &tasks) override;

    std::string toString() const override;

  private:
    NodeToTaskMapping rebuildMapping();
    TaskPtrVec filterOutScheduledTasks(const NodeToTaskMapping &mapping, const TaskPtrVec &tasks);

    NodePtrVec::size_type lastNodeIndex;
    NodeToTaskMapping lastMapping;
};

} // namespace roundrobin
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
