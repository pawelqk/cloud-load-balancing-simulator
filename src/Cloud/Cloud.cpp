#include "Cloud.hpp"

#include <algorithm>
#include <cstdint>

#include "TaskImpl.hpp"

namespace cloud
{

std::uint32_t Cloud::nextTaskId = 0;

Cloud::Cloud(loadbalancer::LoadBalancerPtr &&loadBalancer, const InfrastructurePtr &infrastructure,
             const TimingServicePtr &timingService)
    : loadBalancer(std::move(loadBalancer)), infrastructure(infrastructure), timingService(timingService)
{
}

void Cloud::tick(const instance::TaskDataVec taskDatas)
{
    const auto finishedTasks = infrastructure->advanceProcessing();
    if (!finishedTasks.empty())
        addFlowtime(finishedTasks);

    if (!taskDatas.empty())
        loadBalancer->scheduleNewTasks(createTasks(taskDatas));
    else if (!finishedTasks.empty() && loadBalancer->areAnyTasksWaiting())
        loadBalancer->scheduleWaitingTasks();
}

bool Cloud::isIdle() const
{
    return infrastructure->isIdle() && !loadBalancer->areAnyTasksWaiting();
}

TaskPtrVec Cloud::createTasks(const instance::TaskDataVec taskDatas)
{
    const auto ticks = timingService->getTicks();

    TaskPtrVec tasks;
    tasks.reserve(taskDatas.size());
    for (auto &&taskData : taskDatas)
        tasks.push_back(std::make_shared<TaskImpl>(nextTaskId++, taskData.requiredMips, taskData.length, ticks));

    return tasks;
}

void Cloud::addFlowtime(const TaskPtrVec &finishedTasks)
{
    const auto currentTime = timingService->getTicks();
    std::uint32_t totalFlowTime{0};
    for (auto &&task : finishedTasks)
        totalFlowTime += (currentTime - task->getArrivalTime());

    timingService->addFlowtime(totalFlowTime);
}

} // namespace cloud
