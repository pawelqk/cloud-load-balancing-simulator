#include "Random.hpp"

#include <algorithm>
#include <list>
#include <tuple>
#include <utility>

#include <Utility/RandomNumberGenerator.hpp>

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

Random::Random(const InfrastructureCPtr &infrastructure) : Policy(infrastructure)
{
}

MappingActions Random::buildTaskToNodeMapping(const TaskSet &tasks)
{
    MappingActions mappingActions;
    const auto &nodes = infrastructure->getNodes();

    std::vector<Task> allTasks = {tasks.cbegin(), tasks.cend()};
    TaskSet tasksAssigned;
    std::map<Task, NodeId> currentMapping;
    for (auto &&node : nodes)
    {
        const auto task = node.getTask();
        if (task.has_value())
        {
            currentMapping.emplace(*task, node.getId());
            allTasks.emplace_back(std::move(*task));
        }
    }

    std::vector<std::vector<Task>::size_type> taskIndices(allTasks.size());
    std::iota(taskIndices.begin(), taskIndices.end(), 0);
    std::shuffle(taskIndices.begin(), taskIndices.end(), utility::RandomNumberGenerator::getInstance());

    std::vector<NodeVec::size_type> nodeIndices(nodes.size());
    std::iota(nodeIndices.begin(), nodeIndices.end(), 0);
    std::shuffle(nodeIndices.begin(), nodeIndices.end(), utility::RandomNumberGenerator::getInstance());

    for (auto i = 0u; i < nodes.size() && i < allTasks.size(); ++i)
    {
        const auto taskIndex = taskIndices[i];
        const auto nodeIndex = nodeIndices[i];
        tasksAssigned.insert(allTasks[taskIndex]);

        // to avoid migration to the queue and instantly back to the node
        const auto taskCurrentlyAssignedToNode = nodes[nodeIndex].getTask();
        if (currentMapping.contains(allTasks[taskIndex]) &&
            currentMapping.at(allTasks[taskIndex]) == nodes[nodeIndex].getId())
            continue;

        // automatically throw out previously assigned task from currently selected node back to the queue UNLESS it
        // will be migrated to other Node
        if (taskCurrentlyAssignedToNode.has_value() &&
            !mappingActions.migrations.contains(*taskCurrentlyAssignedToNode))
        {
            mappingActions.migrations.emplace(std::piecewise_construct,
                                              std::forward_as_tuple(*taskCurrentlyAssignedToNode),
                                              std::forward_as_tuple(nodes[nodeIndex].getId(), std::nullopt));
        }

        // as current task will be assigned to something then it no longer needs to be migrated it to the queue
        if (mappingActions.migrations.contains(allTasks[taskIndex]))
            mappingActions.migrations.erase(allTasks[taskIndex]);

        // if current task has already been mapped to something then it is a
        // migration between different nodes
        if (currentMapping.contains(allTasks[taskIndex]))
        {
            mappingActions.migrations.emplace(
                std::piecewise_construct, std::forward_as_tuple(allTasks[taskIndex]),
                std::forward_as_tuple(currentMapping.at(allTasks[taskIndex]), nodes[nodeIndex].getId()));
        }
        else // task has not been mapped - simple assignment
            mappingActions.assignments.emplace(allTasks[taskIndex], nodes[nodeIndex].getId());
    }

    // Rest of tasks that were not assigned have to be explicitly marked as such
    TaskSet notAssignedTasks;
    std::set_difference(tasks.cbegin(), tasks.cend(), tasksAssigned.cbegin(), tasksAssigned.cend(),
                        std::inserter(notAssignedTasks, notAssignedTasks.end()));

    for (auto &&task : notAssignedTasks)
        mappingActions.assignments.emplace(task, std::nullopt);

    return mappingActions;
}

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
