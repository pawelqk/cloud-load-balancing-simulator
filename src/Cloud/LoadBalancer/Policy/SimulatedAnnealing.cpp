#include "SimulatedAnnealing.hpp"

#include <cmath>
#include <random>

#include "Utility/RandomNumberGenerator.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

double assessSolution(const InfrastructureCPtr &infrastructure, const Solution &solution)
{
    std::uint32_t makespan{0};

    const auto &nodes = infrastructure->getNodes();

    for (auto &&[nodeId, tasks] : solution)
    {
        const auto nodeIt =
            std::find_if(nodes.begin(), nodes.end(), [nodeId = nodeId](auto &&node) { return node.getId() == nodeId; });
        if (nodeIt == nodes.end())
            throw std::runtime_error("Cannot find node " + std::to_string(nodeId) + " in solution");

        std::uint32_t nodeMakespan;
        const auto task = nodeIt->getTask();
        if (task != std::nullopt)
            nodeMakespan = task->estimateTimeLeft();
        else
            nodeMakespan = 0;

        for (auto &&task : tasks)
        {
            nodeMakespan += task.estimateTimeLeft();
        }

        if (nodeMakespan > makespan)
            makespan = nodeMakespan;
    }

    return makespan;
}

SimulatedAnnealing::SimulatedAnnealing(const InfrastructureCPtr &infrastructure, const Parameters &parameters)
    : Policy(infrastructure), parameters(parameters), logger("SimulatedAnnealing")
{
}

MappingActions SimulatedAnnealing::buildTaskToNodeMapping(const TaskSet &tasks)
{
    MappingActions mappingActions;

    logger.log("Mapping %u tasks", tasks.size());
    if (tasks != waitingTasks)
    {
        logger.log("New tasks came. Creating new solution");
        solution = createNewSolution(tasks);
        waitingTasks.clear();
        for (auto &&entry : solution)
        {
            waitingTasks.insert(entry.second.cbegin(), entry.second.cend());
        }
    }

    logger.log("New solution created. Creating mapping");
    const auto freeNodeIds = extractFreeNodeIds();
    for (auto &&freeNodeId : freeNodeIds)
    {
        if (solution[freeNodeId].empty())
            continue;

        const auto task = solution[freeNodeId].front();
        waitingTasks.erase(task);
        mappingActions.assignments[task] = freeNodeId;
        solution[freeNodeId].pop_front();
    }

    for (auto &&waitingTask : waitingTasks)
        mappingActions.assignments[waitingTask] = std::nullopt;

    return mappingActions;
}

Solution SimulatedAnnealing::createNewSolution(const TaskSet &tasks)
{
    auto bestSolution = createRandomSolution(tasks);
    if (bestSolution.empty())
        return bestSolution;

    auto nextSolution = bestSolution;
    auto currentSolution = bestSolution;

    auto &rng = utility::RandomNumberGenerator::getInstance();
    std::uniform_real_distribution<> distribution{0, 1.0};

    auto temperature = parameters.startTemperature;

    std::uint32_t numberOfIterations{0};
    logger.log("Starting annealing");
    while (temperature > parameters.endTemperature)
    {
        nextSolution = getNewSolutionFromNeighbourhood(currentSolution);
        const auto nextSolutionValue = assessSolution(infrastructure, nextSolution);
        const auto currentSolutionValue = assessSolution(infrastructure, currentSolution);

        if (nextSolutionValue <= currentSolutionValue)
        {
            currentSolution = nextSolution;
            bestSolution = nextSolution;
        }
        else if (distribution(rng) < std::exp((currentSolutionValue - nextSolutionValue) / temperature))
            currentSolution = nextSolution;

        if ((++numberOfIterations) % parameters.iterationsPerStep == 0)
        {
            temperature *= parameters.coolingRatio;
        }
    }

    return bestSolution;
}

Solution SimulatedAnnealing::createRandomSolution(const TaskSet &tasks)
{
    logger.log("Creating random solution");
    Solution solution;

    std::vector<Task> tasksShuffled = {tasks.cbegin(), tasks.cend()};

    std::vector<std::vector<Task>::size_type> taskIndices(tasksShuffled.size());
    std::iota(taskIndices.begin(), taskIndices.end(), 0);
    std::shuffle(taskIndices.begin(), taskIndices.end(), utility::RandomNumberGenerator::getInstance());

    for (auto &&taskId : taskIndices)
    {
        std::vector<NodeId> possibleNodeIds;
        for (auto &&node : infrastructure->getNodes())
        {
            if (node.canTaskFit(tasksShuffled[taskId]))
            {
                possibleNodeIds.push_back(node.getId());
            }
        }

        std::uniform_int_distribution<> dis(0, possibleNodeIds.size() - 1);
        solution[possibleNodeIds[dis(utility::RandomNumberGenerator::getInstance())]].push_back(tasksShuffled[taskId]);
    }

    return solution;
}

Solution SimulatedAnnealing::getNewSolutionFromNeighbourhood(const Solution &solution)
{
    auto solutionInNeighbourhood = solution;
    auto &rng = utility::RandomNumberGenerator::getInstance();

    auto randomNodeIt = solutionInNeighbourhood.begin();

    while (randomNodeIt->second.empty())
        randomNodeIt = std::next(solutionInNeighbourhood.begin(),
                                 std::uniform_int_distribution<>(0, solutionInNeighbourhood.size() - 1)(rng));

    const auto randomElementIt = std::next(randomNodeIt->second.begin(),
                                           std::uniform_int_distribution<>(0, randomNodeIt->second.size() - 1)(rng));

    const auto randomElement = *randomElementIt;

    randomNodeIt->second.erase(randomElementIt);

    // get subset of tasks that are feasible to put it in
    const auto &nodes = infrastructure->getNodes();
    std::vector<NodeId> feasibleNodeIds;
    for (auto &&[nodeId, tasks] : solutionInNeighbourhood)
    {
        const auto nodeIt =
            std::find_if(nodes.begin(), nodes.end(), [nodeId = nodeId](auto &&node) { return node.getId() == nodeId; });
        if (nodeIt == nodes.end())
            throw std::runtime_error("Cannot find node " + std::to_string(nodeId) + " in solutionInNeighbourhood");

        if (nodeIt->canTaskFit(*randomElementIt))
            feasibleNodeIds.push_back(nodeIt->getId());
    }

    auto &randomNode =
        solutionInNeighbourhood[feasibleNodeIds[std::uniform_int_distribution<>(0, feasibleNodeIds.size() - 1)(rng)]];
    const auto newRandomElementIt =
        randomNode.empty()
            ? randomNode.begin()
            : std::next(randomNode.begin(), std::uniform_int_distribution<>(0, randomNode.size() - 1)(rng));
    randomNode.insert(newRandomElementIt, randomElement);

    return solutionInNeighbourhood;
}

std::vector<NodeId> SimulatedAnnealing::extractFreeNodeIds()
{
    std::vector<NodeId> freeNodeIds;
    for (auto &&node : infrastructure->getNodes())
    {
        if (node.isIdle())
            freeNodeIds.push_back(node.getId());
    }

    return freeNodeIds;
}

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
