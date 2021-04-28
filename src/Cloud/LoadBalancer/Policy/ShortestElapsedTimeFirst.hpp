#pragma once

#include "Cloud/Infrastructure.hpp"
#include "Cloud/Task.hpp"
#include "Logger/Logger.hpp"
#include "PolicyBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

class ShortestElapsedTimeFirst : public PolicyBase
{
  public:
    ShortestElapsedTimeFirst(const InfrastructureCPtr &infrastructure, const logger::LoggerPtr &logger);

    NodeToTaskMapping buildNodeToTaskMapping(const TaskPtrVec &tasks) override;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
