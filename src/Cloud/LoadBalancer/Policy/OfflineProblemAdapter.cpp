#include "OfflineProblemAdapter.hpp"

#include <set>

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

OfflineProblemAdapter::OfflineProblemAdapter(const InfrastructureCPtr &infrastructure,
                                             const NodeToTaskMapping &solution)
    : infrastructure(infrastructure), solution(solution)
{
}

NodeToTaskMapping OfflineProblemAdapter::getNextMapping(const TaskPtrVec &tasks)
{
    auto tasksToMap = tasks;
    const auto &nodes = infrastructure->getNodes();
    for (auto &&node : nodes)
    {
        const auto task = node->getTask();
        if (task != nullptr)
            tasksToMap.push_back(task);
    }

    return mapNextTasks(tasksToMap);
}

NodeToTaskMapping OfflineProblemAdapter::mapNextTasks(const TaskPtrVec &tasks)
{
    const auto taskComparator = [](auto &&leftTask, auto &&rightTask) {
        return leftTask->getId() < rightTask->getId();
    };

    // TODO: what about other order (will preemptions/migrations be considered?)

    std::set<TaskPtr, decltype(taskComparator)> tasksToDistribute{taskComparator};
    for (auto &&task : tasks)
        tasksToDistribute.insert(task);

    NodeToTaskMapping nextMapping;
    for (auto &&[nodeId, tasks] : solution)
    {
        for (auto &&task : tasks)
        {
            if (tasksToDistribute.contains(task))
                nextMapping[nodeId].push_back(task);
        }
    }

    return nextMapping;
}

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
