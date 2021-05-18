#include "OnlineSimulatedAnnealing.hpp"

#include <cmath>
#include <random>

#include "Cloud/LoadBalancer/Policy/ShortestRemainingTimeFirst/ShortestRemainingTimeFirst.hpp"
#include "Utility/RandomNumberGenerator.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace simulatedannealing
{

OnlineSimulatedAnnealing::OnlineSimulatedAnnealing(const InfrastructureCPtr &infrastructure,
                                                   const Parameters &parameters,
                                                   mapping::MappingAssessorPtr &&mappingAssessor,
                                                   const logger::LoggerPtr &logger)
    : SimulatedAnnealingBase(infrastructure, parameters, std::move(mappingAssessor), logger)
{
}

NodeToTaskMapping OnlineSimulatedAnnealing::buildNodeToTaskMappingInternal(const TaskPtrVec &tasks)
{
    logger->debug("Mapping %u tasks", tasks.size());

    auto solution = createNewSolution(tasks);
    return adjustSolutionWithExistingTasks(solution);
}

std::string OnlineSimulatedAnnealing::toString() const
{
    return "OnlineSimulatedAnnealing";
}

NodeToTaskMapping OnlineSimulatedAnnealing::createInitialSolution(const TaskPtrVec &tasks)
{
    logger->debug("Creating random solution");
    if (parameters.initialPopulationGenerationMethod == InitialPopulationGenerationMethod::SRTF)
        return shortestremainingtimefirst::ShortestRemainingTimeFirst{infrastructure, logger}.buildStartingSolution(
            tasks);
    else
    {
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
}

NodeToTaskMapping OnlineSimulatedAnnealing::getNewSolutionFromNeighbourhood(const NodeToTaskMapping &solution)
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

NodeToTaskMapping OnlineSimulatedAnnealing::adjustSolutionWithExistingTasks(const NodeToTaskMapping &solution)
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

} // namespace simulatedannealing
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
