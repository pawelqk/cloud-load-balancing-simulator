#include "HeuristicPolicyWithMigrationsAndPreemptions.hpp"

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <stdexcept>

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

HeuristicPolicyWithMigrationsAndPreemptions::HeuristicPolicyWithMigrationsAndPreemptions(
    const InfrastructureCPtr &infrastructure, const logger::LoggerPtr &logger, const bool withMigrationsFixing)
    : PolicyBase(infrastructure, logger), withMigrationsFixing(withMigrationsFixing)
{
}

NodeToTaskMapping HeuristicPolicyWithMigrationsAndPreemptions::buildNodeToTaskMapping(const TaskPtrVec &tasks)
{
    const auto solution = withMigrationsFixing ? buildWithMigrationsFixing(tasks) : build(tasks);

    if (!validateSolution(tasks, solution))
        throw std::runtime_error("Something is really wrong with the algorithm!");

    return solution;
}

NodeToTaskMapping HeuristicPolicyWithMigrationsAndPreemptions::buildWithMigrationsFixing(const TaskPtrVec &tasks)
{
    NodeToTaskMapping solution;
    auto tasksSortedByShortestRemainingTime = tasks;

    auto &nodes = infrastructure->getNodes();
    std::map<NodeId, std::uint32_t> nodesRemainingTimes;
    std::map<TaskPtr, NodeId> oldMapping;
    for (auto &&node : nodes)
    {
        const auto task = node->getTask();
        nodesRemainingTimes[node->getId()] = 0;
        if (task != nullptr)
        {
            tasksSortedByShortestRemainingTime.push_back(task);
            oldMapping[task] = node->getId();
        }
    }

    std::sort(tasksSortedByShortestRemainingTime.begin(), tasksSortedByShortestRemainingTime.end(),
              [this](auto &&lhs, auto &&rhs) { return heuristic(lhs, rhs); });

    std::map<TaskPtr, NodeId> newMapping;
    for (auto i = 0u; i < nodes.size() && i < tasksSortedByShortestRemainingTime.size(); ++i)
    {
        NodePtrVec feasibleNodes;
        std::copy_if(nodes.cbegin(), nodes.cend(), std::back_inserter(feasibleNodes),
                     [task = tasksSortedByShortestRemainingTime[i]](auto &&node) { return node->canTaskFit(task); });

        const auto node = std::min_element(
            feasibleNodes.begin(), feasibleNodes.end(), [&nodesRemainingTimes](auto &&leftNode, auto &&rightNode) {
                return nodesRemainingTimes[leftNode->getId()] < nodesRemainingTimes[rightNode->getId()];
            });

        solution[(*node)->getId()].push_back(tasksSortedByShortestRemainingTime[i]);
        nodesRemainingTimes[(*node)->getId()] += tasksSortedByShortestRemainingTime[i]->estimateTimeLeft();
        newMapping[tasksSortedByShortestRemainingTime[i]] = (*node)->getId();
        logger->debug("Handling %s to %u", tasksSortedByShortestRemainingTime[i]->toString().c_str(), (*node)->getId());
    }

    for (auto &&[task, nodeId] : oldMapping)
    {
        if (newMapping.contains(task) && oldMapping[task] != newMapping[task])
        {
            const auto newNodeIt = std::find_if(nodes.begin(), nodes.end(), [nodeId = newMapping[task]](auto &&node) {
                return node->getId() == nodeId;
            });
            if (newNodeIt == nodes.end())
                throw std::runtime_error("Cannot find node " + std::to_string(nodeId) + " in solutionInNeighbourhood");

            // TODO: write test for this if
            if (solution[oldMapping[task]].empty())
            {
                logger->debug("Empty %s", task->toString().c_str());

                solution[oldMapping[task]].push_back(task);
                nodesRemainingTimes[oldMapping[task]] += task->estimateTimeLeft();

                solution[newMapping[task]].remove(task);
                nodesRemainingTimes[newMapping[task]] -= task->estimateTimeLeft();

                newMapping[task] = oldMapping[task];
            }
            else if (solution[oldMapping[task]].size() == 1)
            {
                auto taskOnOldMapping = solution[oldMapping[task]].front();

                if ((*newNodeIt)->canTaskFit(taskOnOldMapping))
                {
                    logger->debug("Changing %s from %u to %u", task->toString().c_str(), newMapping[task],
                                  oldMapping[task]);
                    logger->debug("And Changing %s from %u to %u", taskOnOldMapping->toString().c_str(),
                                  oldMapping[task], newMapping[task]);

                    solution[oldMapping[task]].pop_front();
                    solution[oldMapping[task]].push_back(task);
                    nodesRemainingTimes[oldMapping[task]] -= taskOnOldMapping->estimateTimeLeft();
                    nodesRemainingTimes[oldMapping[task]] += task->estimateTimeLeft();

                    auto taskIt = std::find(solution[newMapping[task]].begin(), solution[newMapping[task]].end(), task);
                    taskIt = solution[newMapping[task]].erase(taskIt);
                    solution[newMapping[task]].insert(taskIt, taskOnOldMapping);
                    nodesRemainingTimes[newMapping[task]] -= task->estimateTimeLeft();
                    nodesRemainingTimes[newMapping[task]] += taskOnOldMapping->estimateTimeLeft();

                    newMapping[taskOnOldMapping] = newMapping[task];
                    newMapping[task] = oldMapping[task];
                }
            }
        }
    }

    for (auto i = nodes.size(); i < tasksSortedByShortestRemainingTime.size(); ++i)
    {
        logger->debug("Standard Handling %s", tasksSortedByShortestRemainingTime[i]->toString().c_str());
        NodePtrVec feasibleNodes;
        std::copy_if(nodes.cbegin(), nodes.cend(), std::back_inserter(feasibleNodes),
                     [task = tasksSortedByShortestRemainingTime[i]](auto &&node) { return node->canTaskFit(task); });

        const auto node = std::min_element(
            feasibleNodes.begin(), feasibleNodes.end(), [&nodesRemainingTimes](auto &&leftNode, auto &&rightNode) {
                return nodesRemainingTimes[leftNode->getId()] < nodesRemainingTimes[rightNode->getId()];
            });

        solution[(*node)->getId()].push_back(tasksSortedByShortestRemainingTime[i]);
        nodesRemainingTimes[(*node)->getId()] += tasksSortedByShortestRemainingTime[i]->estimateTimeLeft();
    }

    return solution;
}

NodeToTaskMapping HeuristicPolicyWithMigrationsAndPreemptions::build(const TaskPtrVec &tasks)
{
    NodeToTaskMapping solution;
    auto tasksSortedByShortestRemainingTime = tasks;

    auto &nodes = infrastructure->getNodes();
    std::map<NodeId, std::uint32_t> nodesRemainingTimes;
    for (auto &&node : nodes)
    {
        const auto task = node->getTask();
        nodesRemainingTimes[node->getId()] = 0;
        if (task != nullptr)
            tasksSortedByShortestRemainingTime.push_back(task);
    }

    std::sort(tasksSortedByShortestRemainingTime.begin(), tasksSortedByShortestRemainingTime.end(),
              [this](auto &&lhs, auto &&rhs) { return heuristic(lhs, rhs); });

    for (auto i = 0u; i < tasksSortedByShortestRemainingTime.size(); ++i)
    {
        logger->debug("Standard Handling %s", tasksSortedByShortestRemainingTime[i]->toString().c_str());
        NodePtrVec feasibleNodes;
        std::copy_if(nodes.cbegin(), nodes.cend(), std::back_inserter(feasibleNodes),
                     [task = tasksSortedByShortestRemainingTime[i]](auto &&node) { return node->canTaskFit(task); });

        const auto node = std::min_element(
            feasibleNodes.begin(), feasibleNodes.end(), [&nodesRemainingTimes](auto &&leftNode, auto &&rightNode) {
                return nodesRemainingTimes[leftNode->getId()] < nodesRemainingTimes[rightNode->getId()];
            });

        solution[(*node)->getId()].push_back(tasksSortedByShortestRemainingTime[i]);
        nodesRemainingTimes[(*node)->getId()] += tasksSortedByShortestRemainingTime[i]->estimateTimeLeft();
    }

    return solution;
}

bool HeuristicPolicyWithMigrationsAndPreemptions::validateSolution(const TaskPtrVec &insertedTasks,
                                                                   const NodeToTaskMapping &mapping)
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
