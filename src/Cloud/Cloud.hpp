#pragma once

#include <vector>

#include "Infrastructure.hpp"
#include "LoadBalancer/LoadBalancer.hpp"
#include "Node.hpp"
#include "Task.hpp"

namespace cloud
{

class Cloud
{
  public:
    Cloud(loadbalancer::LoadBalancerPtr &&loadBalancer, const InfrastructurePtr &infrastructure);

    void tick(const std::vector<Task> &tasks);

    bool isIdle() const;

  private:
    const loadbalancer::LoadBalancerPtr loadBalancer;
    const InfrastructurePtr infrastructure;
};

} // namespace cloud
