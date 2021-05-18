#include "PolicyBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

PolicyBase::PolicyBase(const InfrastructureCPtr &infrastructure, const logger::LoggerPtr &logger)
    : infrastructure(infrastructure), logger(logger)
{
}

NodeToTaskMapping PolicyBase::buildNodeToTaskMapping(const TaskPtrVec &tasks)
{
    const auto solution = buildNodeToTaskMappingInternal(tasks);
    if (!validateSolution(tasks, solution))
        throw PolicyException{"Invalid solution created by " + toString()};
    else
        logger->debug("Proper solution");

    return solution;
}

bool PolicyBase::validateSolution(const TaskPtrVec &insertedTasks, const NodeToTaskMapping &mapping)
{
    logger->debug("Validating %s", ::cloud::loadbalancer::policy::toString(mapping).c_str());

    std::vector<std::uint32_t> allTaskIds;
    for (auto &&task : insertedTasks)
        allTaskIds.push_back(task->getId());

    auto &nodes = infrastructure->getNodes();
    for (auto &&node : nodes)
    {
        const auto task = node->getTask();
        if (task != nullptr)
            allTaskIds.push_back(task->getId());
    }

    std::sort(allTaskIds.begin(), allTaskIds.end());
    std::vector<std::uint32_t> mappedTaskIds;
    for (auto &&entry : mapping)
    {
        for (auto &&task : entry.second)
            mappedTaskIds.push_back(task->getId());
    }

    std::sort(mappedTaskIds.begin(), mappedTaskIds.end());

    return allTaskIds == mappedTaskIds;
}

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
