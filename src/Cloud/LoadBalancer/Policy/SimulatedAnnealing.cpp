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

SimulatedAnnealing::SimulatedAnnealing(const InfrastructureCPtr &infrastructure, Parameters &&parameters,
                                       const logger::LoggerPtr &logger)
    : PolicyBase(infrastructure), parameters(std::move(parameters)), logger(logger)
{
}

NodeToTaskMapping SimulatedAnnealing::buildNodeToTaskMapping(const TaskPtrVec &tasks)
{
    logger->debug("Mapping %u tasks", tasks.size());

    const auto solution = createNewSolution(tasks);

    return solution;
}

NodeToTaskMapping SimulatedAnnealing::createNewSolution(const TaskPtrVec &tasks)
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

    logger->debug("Starting annealing");
    while (temperature > parameters.endTemperature)
    {
        nextSolution = getNewSolutionFromNeighbourhood(currentSolution);
        const auto nextSolutionValue = parameters.mappingAssessor->assess(nextSolution);
        const auto currentSolutionValue = parameters.mappingAssessor->assess(currentSolution);

        if (nextSolutionValue <= currentSolutionValue)
        {
            currentSolution = nextSolution;
            bestSolution = nextSolution;
        }
        else if (distribution(rng) < std::exp((currentSolutionValue - nextSolutionValue) / temperature))
            currentSolution = nextSolution;

        if ((++numberOfIterations) % parameters.iterationsPerStep == 0)
            temperature *= parameters.coolingRatio;
    }

    return bestSolution;
}

NodeToTaskMapping SimulatedAnnealing::createRandomSolution(const TaskPtrVec &tasks)
{
    logger->debug("Creating random solution");
    NodeToTaskMapping solution;

    auto tasksShuffled = tasks;
    std::shuffle(tasksShuffled.begin(), tasksShuffled.end(), utility::RandomNumberGenerator::getInstance());

    for (auto &&task : tasksShuffled)
    {
        std::vector<NodeId> possibleNodeIds;
        for (auto &&node : infrastructure->getNodes())
        {
            if (node->canTaskFit(task))
                possibleNodeIds.push_back(node->getId());
        }

        std::uniform_int_distribution<> dis(0, possibleNodeIds.size() - 1);
        solution[possibleNodeIds[dis(utility::RandomNumberGenerator::getInstance())]].push_back(task);
    }

    return solution;
}

NodeToTaskMapping SimulatedAnnealing::getNewSolutionFromNeighbourhood(const NodeToTaskMapping &solution)
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
        const auto nodeIt = std::find_if(nodes.begin(), nodes.end(),
                                         [nodeId = nodeId](auto &&node) { return node->getId() == nodeId; });
        if (nodeIt == nodes.end())
            throw std::runtime_error("Cannot find node " + std::to_string(nodeId) + " in solutionInNeighbourhood");

        if ((*nodeIt)->canTaskFit(*randomElementIt))
            feasibleNodeIds.push_back((*nodeIt)->getId());
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
        if (node->isIdle())
            freeNodeIds.push_back(node->getId());
    }

    return freeNodeIds;
}

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
