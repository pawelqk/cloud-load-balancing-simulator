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

    virtual void schedule(const TaskSet &tasks) = 0;
    virtual void tick() = 0;
    virtual bool isIdle() const = 0;
};

using LoadBalancerPtr = std::unique_ptr<LoadBalancer>;

} // namespace loadbalancer
} // namespace cloud
