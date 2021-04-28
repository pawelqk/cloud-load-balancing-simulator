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

class RoundRobin : public PolicyBase
{
  public:
    RoundRobin(const InfrastructureCPtr &infrastructure, const logger::LoggerPtr &logger);

    NodeToTaskMapping buildNodeToTaskMapping(const TaskPtrVec &tasks) override;

    std::string toString() const override;

  private:
    NodePtrVec::size_type lastNodeIndex;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
