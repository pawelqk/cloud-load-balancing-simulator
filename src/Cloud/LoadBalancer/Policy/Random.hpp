#pragma once

#include <map>
#include <memory>
#include <optional>
#include <vector>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/Node.hpp"
#include "Cloud/Task.hpp"
#include "PolicyBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

class Random : public PolicyBase
{
  public:
    Random(const InfrastructureCPtr &infrastructure);

    NodeToTaskMapping buildNodeToTaskMapping(const TaskPtrVec &tasks) override;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
