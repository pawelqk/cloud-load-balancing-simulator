#pragma once

#include <string>

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
namespace random
{

class Random : public PolicyBase
{
  public:
    Random(const InfrastructureCPtr &infrastructure, const logger::LoggerPtr &logger);

    NodeToTaskMapping buildNodeToTaskMappingInternal(const TaskPtrVec &tasks) override;

    std::string toString() const override;

  private:
    NodeToTaskMapping adjustSolutionWithExistingTasks(const NodeToTaskMapping &solution);

    const logger::LoggerPtr logger;
};

} // namespace random
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
