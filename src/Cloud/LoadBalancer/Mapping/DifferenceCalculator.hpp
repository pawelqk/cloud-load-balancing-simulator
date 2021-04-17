#pragma once

#include <functional>
#include <memory>

#include "Cloud/LoadBalancer/Policy/Policy.hpp"
#include "Cloud/Node.hpp"
#include "Cloud/Task.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{

struct Migration
{
    NodeId source;
    NodeId destination;
    TaskPtr task;
};

struct Preemption
{
    NodeId source;
    TaskPtr task;
};

struct Difference
{
    std::vector<Migration> migrations;
    std::vector<Preemption> preemptions;
    std::vector<NodeId> nodeIdsWithoutChange;
};

bool operator==(const Migration &lhs, const Migration &rhs);
bool operator==(const Preemption &lhs, const Preemption &rhs);
bool operator==(const Difference &lhs, const Difference &rhs);

class DifferenceCalculator
{
  public:
    virtual ~DifferenceCalculator();

    virtual Difference calculate(const policy::NodeToTaskMapping &) const = 0;
};

using DifferenceCalculatorPtr = std::shared_ptr<DifferenceCalculator>;

} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
