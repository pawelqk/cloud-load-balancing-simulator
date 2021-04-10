#include "Cloud.hpp"

#include <algorithm>

namespace cloud
{

Cloud::Cloud(loadbalancer::LoadBalancerPtr &&loadBalancer, const InfrastructurePtr &infrastructure)
    : loadBalancer(std::move(loadBalancer)), infrastructure(infrastructure)
{
}

void Cloud::tick(const std::vector<Task> &tasks)
{
    const auto finishedTasks = infrastructure->advanceProcessing();
    if (!tasks.empty())
        loadBalancer->scheduleNewTasks({tasks.cbegin(), tasks.cend()});
    else if (!finishedTasks.empty() && loadBalancer->areAnyTasksWaiting())
        loadBalancer->scheduleWaitingTasks();
}

bool Cloud::isIdle() const
{
    return infrastructure->isIdle() && !loadBalancer->areAnyTasksWaiting();
}

} // namespace cloud
