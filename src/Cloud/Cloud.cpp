#include "Cloud.hpp"

#include <algorithm>

namespace cloud
{

Cloud::Cloud(loadbalancer::LoadBalancerPtr &&loadBalancer) : loadBalancer(std::move(loadBalancer))
{
}

void Cloud::insertTasks(const std::vector<Task> &tasks)
{
    loadBalancer->schedule({tasks.cbegin(), tasks.cend()});
}

void Cloud::tick()
{
    loadBalancer->tick();
}

bool Cloud::isIdle() const
{
    return loadBalancer->isIdle();
}

} // namespace cloud
