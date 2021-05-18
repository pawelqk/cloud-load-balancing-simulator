#include "ArtificialBeeColonyBase.hpp"

#include <sstream>
#include <utility>

#include "Utility/RandomNumberGenerator.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace artificialbeecolony
{

std::string toString(const Parameters &parameters)
{
    std::stringstream ss;
    ss << parameters.populationSize << "_" << parameters.maxIterationsWithoutChange << "_" << parameters.maxIterations;

    return ss.str();
}

ArtificialBeeColonyBase::ArtificialBeeColonyBase(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                                                 mapping::MappingAssessorPtr &&mappingAssessor,
                                                 const logger::LoggerPtr &logger)
    : PolicyBase(infrastructure, logger), parameters(parameters), mappingAssessor(std::move(mappingAssessor)),
      solutions(parameters.populationSize)
{
}

NodeToTaskMapping ArtificialBeeColonyBase::createNewSolution(const TaskPtrVec &tasks)
{
    auto bestSolution = generateRandomSolutions(tasks);

    for (auto iteration = 0u; iteration < parameters.maxIterations; ++iteration)
    {
        // employed bees
        for (auto i = 0u; i < parameters.populationSize; ++i)
        {
            auto &currentSolution = solutions[i];
            const auto nextSolution = getNewSolutionFromNeighbourhood(currentSolution.solution);
            const auto nextSolutionValue = mappingAssessor->assess(nextSolution);
            const auto currentSolutionValue = mappingAssessor->assess(currentSolution.solution);

            if (nextSolutionValue < currentSolutionValue)
            {
                currentSolution.counter = 0;
                currentSolution.solution = nextSolution;
                if (nextSolutionValue < bestSolution.value)
                {
                    bestSolution.solution = nextSolution;
                    bestSolution.value = nextSolutionValue;
                }
            }
            else
                ++currentSolution.counter;
        }

        std::vector<double> weights(parameters.populationSize);
        for (auto i = 0u; i < parameters.populationSize; ++i)
        {
            const auto solutionValue = mappingAssessor->assess(solutions[i].solution);
            weights[i] = 1.0 / solutionValue;
        }

        auto &rng = utility::RandomNumberGenerator::getInstance();
        std::discrete_distribution dist(weights.cbegin(), weights.cend());

        // onlookers
        for (auto i = 0u; i < parameters.populationSize; ++i)
        {
            // TODO: extract to common code
            auto &currentSolution = solutions[dist(rng)];
            const auto nextSolution = getNewSolutionFromNeighbourhood(currentSolution.solution);
            const auto nextSolutionValue = mappingAssessor->assess(nextSolution);
            const auto currentSolutionValue = mappingAssessor->assess(currentSolution.solution);

            if (nextSolutionValue < currentSolutionValue)
            {
                currentSolution.counter = 0;
                currentSolution.solution = nextSolution;
                if (nextSolutionValue < bestSolution.value)
                {
                    bestSolution.solution = nextSolution;
                    bestSolution.value = nextSolutionValue;
                }
            }
            else
                ++currentSolution.counter;
        }

        // scouts
        for (auto i = 0u; i < parameters.populationSize; ++i)
        {
            if (solutions[i].counter > parameters.maxIterationsWithoutChange)
            {
                // TODO: extract to common code?? (looks similar to generateRandomSolutions)
                solutions[i].solution = createRandomSolution(tasks);
                solutions[i].counter = 0;
                const auto currentValue = mappingAssessor->assess(solutions[i].solution);
                if (currentValue < bestSolution.value)
                {
                    bestSolution.solution = solutions[i].solution;
                    bestSolution.value = currentValue;
                }
            }
        }
    }

    return bestSolution.solution;
}

ArtificialBeeColonyBase::BestSolution ArtificialBeeColonyBase::generateRandomSolutions(const TaskPtrVec &tasks)
{
    solutions[0].solution = createRandomSolution(tasks);
    solutions[0].counter = 0;
    auto bestSolution = solutions[0].solution;
    auto bestValue = mappingAssessor->assess(bestSolution);

    for (auto i = 1u; i < parameters.populationSize; ++i)
    {
        solutions[i].solution = createRandomSolution(tasks);
        solutions[i].counter = 0;
        const auto currentValue = mappingAssessor->assess(solutions[i].solution);
        if (currentValue < bestValue)
        {
            bestSolution = solutions[i].solution;
            bestValue = currentValue;
        }
    }

    return {bestSolution, bestValue};
}

NodeToTaskMapping ArtificialBeeColonyBase::createRandomSolution(const TaskPtrVec &tasks)
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

NodeToTaskMapping ArtificialBeeColonyBase::getNewSolutionFromNeighbourhood(const NodeToTaskMapping &solution)
{
    auto solutionInNeighbourhood = solution;
    auto &rng = utility::RandomNumberGenerator::getInstance();

    const auto notEmptyNodeIds = findNotEmptyNodeIds(solution);
    const auto randomNotEmptyNodeId =
        notEmptyNodeIds[std::uniform_int_distribution<>(0, notEmptyNodeIds.size() - 1)(rng)];
    const auto randomSourceNodeIt = solutionInNeighbourhood.find(randomNotEmptyNodeId);
    if (randomSourceNodeIt == solutionInNeighbourhood.end())
        throw std::runtime_error("Cannot find node " + std::to_string(randomNotEmptyNodeId) +
                                 " in solutionInNeighbourhood");

    const auto movedElementSourceIt =
        std::next(randomSourceNodeIt->second.begin(),
                  std::uniform_int_distribution<>(0, randomSourceNodeIt->second.size() - 1)(rng));
    const auto movedElement = *movedElementSourceIt;
    randomSourceNodeIt->second.erase(movedElementSourceIt);

    const auto feasibleNodeIds = findFeasibleNodeIds(solution, movedElement);
    auto &randomDestinationNode =
        solutionInNeighbourhood[feasibleNodeIds[std::uniform_int_distribution<>(0, feasibleNodeIds.size() - 1)(rng)]];
    const auto movedElementDestinationIt =
        randomDestinationNode.empty()
            ? randomDestinationNode.begin()
            : std::next(randomDestinationNode.begin(),
                        std::uniform_int_distribution<>(0, randomDestinationNode.size() - 1)(rng));

    randomDestinationNode.insert(movedElementDestinationIt, movedElement);

    return solutionInNeighbourhood;
}

NodeToTaskMapping ArtificialBeeColonyBase::adjustSolutionWithExistingTasks(const NodeToTaskMapping &solution)
{
    auto adjustedSolution = solution;

    const auto &nodes = infrastructure->getNodes();
    for (auto &&node : nodes)
    {
        const auto task = node->getTask();
        if (task != nullptr)
            adjustedSolution[node->getId()].push_front(task);
    }

    return adjustedSolution;
}

std::vector<NodeId> ArtificialBeeColonyBase::extractFreeNodeIds()
{
    std::vector<NodeId> freeNodeIds;
    for (auto &&node : infrastructure->getNodes())
    {
        if (node->isIdle())
            freeNodeIds.push_back(node->getId());
    }

    return freeNodeIds;
}

std::vector<NodeId> ArtificialBeeColonyBase::findNotEmptyNodeIds(const NodeToTaskMapping &solution)
{
    std::vector<NodeId> notEmptyNodeIds;
    for (auto &&entry : solution)
    {
        if (!entry.second.empty())
            notEmptyNodeIds.push_back(entry.first);
    }

    return notEmptyNodeIds;
}

std::vector<NodeId> ArtificialBeeColonyBase::findFeasibleNodeIds(const NodeToTaskMapping &solution, const TaskPtr &task)
{
    std::vector<NodeId> feasibleNodeIds;

    const auto &nodes = infrastructure->getNodes();
    for (auto &&[nodeId, _] : solution)
    {
        const auto nodeIt = std::find_if(nodes.begin(), nodes.end(),
                                         [nodeId = nodeId](auto &&node) { return node->getId() == nodeId; });
        if (nodeIt == nodes.end())
            throw std::runtime_error("Cannot find node " + std::to_string(nodeId) + " in solutionInNeighbourhood");

        if ((*nodeIt)->canTaskFit(task))
            feasibleNodeIds.push_back((*nodeIt)->getId());
    }

    return feasibleNodeIds;
}

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
