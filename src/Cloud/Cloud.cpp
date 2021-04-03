#include "Cloud.hpp"

#include <algorithm>

namespace cloud
{

Cloud::Cloud(loadbalancer::LoadBalancerPtr &&loadBalancer, const InfrastructurePtr &infrastructure)
    : loadBalancer(std::move(loadBalancer)), infrastructure(infrastructure)
{
}

void Cloud::insertTasks(const std::vector<Task> &tasks)
{
    loadBalancer->schedule({tasks.cbegin(), tasks.cend()});
}

void Cloud::tick()
{
    const auto finishedTasks = infrastructure->advanceProcessing();
    if (!finishedTasks.empty() && loadBalancer->areAnyTasksWaiting())
        loadBalancer->schedule({});
}

bool Cloud::isIdle() const
{
    return infrastructure->isIdle() && !loadBalancer->areAnyTasksWaiting();
}

} // namespace cloud
