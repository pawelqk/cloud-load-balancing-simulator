#include "Cloud.hpp"

#include <algorithm>
#include <cstdint>
#include <sstream>

#include "TaskImpl.hpp"

namespace cloud
{

Cloud::Cloud(loadbalancer::LoadBalancerPtr &&loadBalancer, const InfrastructurePtr &infrastructure,
             const TimingServicePtr &timingService, const double penaltyFactor, const logger::LoggerPtr &logger)
    : loadBalancer(std::move(loadBalancer)), infrastructure(infrastructure), timingService(timingService),
      penaltyFactor(penaltyFactor), logger(logger)
{
}

void Cloud::tick(const configuration::TaskDataVec taskDatas)
{
    const auto finishedTasks = infrastructure->advanceProcessing();
    if (!finishedTasks.empty())
    {
        addFlowtime(finishedTasks);
        saveFinishedTasks(finishedTasks);
    }

    if (!taskDatas.empty())
        loadBalancer->scheduleNewTasks(createTasks(taskDatas));
    else if (!finishedTasks.empty() && loadBalancer->areAnyTasksWaiting())
        loadBalancer->scheduleWaitingTasks();
}

bool Cloud::isIdle() const
{
    return infrastructure->isIdle() && !loadBalancer->areAnyTasksWaiting();
}

std::string Cloud::toString() const
{
    return loadBalancer->toString();
}

bool Cloud::areAllInsertedTasksFinished() const
{
    return insertedTaskIds == finishedTaskIds;
}

TaskPtrVec Cloud::createTasks(const configuration::TaskDataVec taskDatas)
{
    const auto ticks = timingService->getTicks();

    TaskPtrVec tasks;
    tasks.reserve(taskDatas.size());
    for (auto &&taskData : taskDatas)
    {
        tasks.push_back(
            std::make_shared<TaskImpl>(taskData.id, taskData.requiredMips, taskData.length, ticks, penaltyFactor));
        insertedTaskIds.insert(taskData.id);
    }

    logNewTasks(tasks);

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

void Cloud::saveFinishedTasks(const TaskPtrVec &finishedTasks)
{
    for (auto &&task : finishedTasks)
        finishedTaskIds.insert(task->getId());
}

void Cloud::logNewTasks(const TaskPtrVec &tasks)
{
    std::stringstream ss;
    ss << "Inserted new tasks:\n";
    for (auto &&task : tasks)
        ss << "id: " << task->getId() << ", mips: " << task->getMips() << ", length: " << task->getInitialLength()
           << "\n";

    auto output = ss.str();

    const auto newLineToDeletePos = output.rfind('\n');
    if (newLineToDeletePos != std::string::npos)
        output.erase(newLineToDeletePos);

    logger->debug("%s", output.c_str());
}

} // namespace cloud
