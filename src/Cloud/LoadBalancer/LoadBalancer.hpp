#pragma once

#include <map>
#include <memory>
#include <vector>

#include "../Node.hpp"
#include "../Task.hpp"

namespace cloud
{
namespace loadbalancer
{

class LoadBalancer
{
  public:
    virtual ~LoadBalancer() = default;

    virtual void scheduleNewTasks(const TaskPtrVec &tasks) = 0;
    virtual void scheduleWaitingTasks() = 0;
    virtual bool areAnyTasksWaiting() const = 0;
    virtual std::string toString() const = 0;
};

using LoadBalancerPtr = std::unique_ptr<LoadBalancer>;

} // namespace loadbalancer
} // namespace cloud
