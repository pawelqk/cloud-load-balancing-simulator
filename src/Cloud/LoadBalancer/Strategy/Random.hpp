#pragma once

#include <map>
#include <memory>
#include <optional>
#include <vector>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/Node.hpp"
#include "Cloud/Task.hpp"
#include "Strategy.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace strategy
{

class Random : public Strategy
{
  public:
    Random(const InfrastructureCPtr &infrastructure);

    MappingActions buildTaskToNodeMapping(const TaskSet &tasks) override;
};

} // namespace strategy
} // namespace loadbalancer
} // namespace cloud
