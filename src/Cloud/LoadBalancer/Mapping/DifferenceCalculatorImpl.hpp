#pragma once

#include <functional>
#include <memory>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Policy/Policy.hpp"
#include "Cloud/Node.hpp"
#include "Cloud/Task.hpp"
#include "DifferenceCalculator.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{

class DifferenceCalculatorImpl : public DifferenceCalculator
{
  public:
    DifferenceCalculatorImpl(const InfrastructureCPtr &Infrastructure);

    Difference calculate(const policy::NodeToTaskMapping &mapping) const override;

  private:
    using TaskToNode = std::map<TaskPtr, NodeId>;

    Difference calculateDifference(const TaskToNode &currentMapping, const TaskToNode &newMapping) const;

    const InfrastructureCPtr Infrastructure;
};

} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
