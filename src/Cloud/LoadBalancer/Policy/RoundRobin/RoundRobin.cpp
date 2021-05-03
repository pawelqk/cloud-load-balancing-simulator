#include "RoundRobin.hpp"
#include "Cloud/LoadBalancer/Policy/Policy.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace roundrobin
{

RoundRobin::RoundRobin(const InfrastructureCPtr &infrastructure, const logger::LoggerPtr &logger)
    : PolicyBase(infrastructure, logger), lastNodeIndex(0), lastMapping({})
{
}

NodeToTaskMapping RoundRobin::buildNodeToTaskMapping(const TaskPtrVec &tasks)
{
    NodeToTaskMapping solution = rebuildMapping();
    const auto tasksToSchedule = filterOutScheduledTasks(solution, tasks);

    const auto &nodes = infrastructure->getNodes();

    for (auto &&task : tasksToSchedule)
    {
        for (auto i = 0u; i < nodes.size(); ++i)
        {
            const auto &node = nodes[lastNodeIndex];
            lastNodeIndex = (lastNodeIndex + 1) % nodes.size();

            if (node->canTaskFit(task))
            {
                solution[node->getId()].push_back(task);
                break;
            }
        }
    }

    lastMapping = solution;

    return solution;
}

std::string RoundRobin::toString() const
{
    return "RoundRobin";
}

NodeToTaskMapping RoundRobin::rebuildMapping()
{
    NodeToTaskMapping rebuiltMapping;
    logger->info("rebuilding. old node mapping: %s", cloud::loadbalancer::policy::toString(lastMapping).c_str());

    auto &nodes = infrastructure->getNodes();
    for (auto &&node : nodes)
    {
        const auto task = node->getTask();
        if (task != nullptr)
        {
            auto &oldNodeMapping = lastMapping[node->getId()];
            const auto taskIt = std::find(oldNodeMapping.cbegin(), oldNodeMapping.cend(), task);
            if (taskIt == oldNodeMapping.end())
                throw std::runtime_error("A task disappeared from mapping ");

            auto &newNodeMapping = rebuiltMapping[node->getId()];
            newNodeMapping.splice(newNodeMapping.begin(), oldNodeMapping, taskIt, oldNodeMapping.end());
        }
        else
            rebuiltMapping[node->getId()] = {};
    }

    return rebuiltMapping;
}

TaskPtrVec RoundRobin::filterOutScheduledTasks(const NodeToTaskMapping &mapping, const TaskPtrVec &tasks)
{
    std::set<std::uint32_t> scheduledTaskIds;
    for (auto &&entry : mapping)
    {
        for (auto &&task : entry.second)
            scheduledTaskIds.insert(task->getId());
    }

    auto tasksWithoutScheduled = tasks;
    tasksWithoutScheduled.erase(
        std::remove_if(tasksWithoutScheduled.begin(), tasksWithoutScheduled.end(),
                       [&scheduledTaskIds](auto &&task) { return scheduledTaskIds.contains(task->getId()); }),
        tasksWithoutScheduled.end());

    return tasksWithoutScheduled;
}

} // namespace roundrobin
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
