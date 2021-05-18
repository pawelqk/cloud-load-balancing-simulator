#include "OnlineSimulatedAnnealingWithMigrationsAndPreemptions.hpp"
#include "Cloud/LoadBalancer/Policy/ShortestRemainingTimeFirst/ShortestRemainingTimeFirstWithMigrationsAndPreemptions.hpp"

#include <cmath>
#include <random>

#include "Utility/RandomNumberGenerator.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace simulatedannealing
{

OnlineSimulatedAnnealingWithMigrationsAndPreemptions::OnlineSimulatedAnnealingWithMigrationsAndPreemptions(
    const InfrastructureCPtr &infrastructure, const Parameters &parameters,
    mapping::MappingAssessorPtr &&mappingAssessor, const logger::LoggerPtr &logger)
    : SimulatedAnnealingBase(infrastructure, parameters, std::move(mappingAssessor), logger)
{
}

NodeToTaskMapping OnlineSimulatedAnnealingWithMigrationsAndPreemptions::buildNodeToTaskMappingInternal(
    const TaskPtrVec &tasks)
{
    logger->debug("Mapping %u tasks", tasks.size());

    auto allTasks = tasks;
    for (auto &&node : infrastructure->getNodes())
    {
        const auto task = node->getTask();
        if (task != nullptr)
            allTasks.push_back(task);
    }

    const auto solution = createNewSolution(allTasks);

    return solution;
}

std::string OnlineSimulatedAnnealingWithMigrationsAndPreemptions::toString() const
{
    return "OnlineSimulatedAnnealingWithMigrationsAndPreemptions";
}

NodeToTaskMapping OnlineSimulatedAnnealingWithMigrationsAndPreemptions::createInitialSolution(const TaskPtrVec &tasks)
{
    logger->debug("Creating random solution");
    if (parameters.initialPopulationGenerationMethod == InitialPopulationGenerationMethod::SRTF)
        return shortestremainingtimefirst::ShortestRemainingTimeFirstWithMigrationsAndPreemptions{infrastructure,
                                                                                                  logger, true}
            .buildNodeToTaskMappingInternal(tasks);
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

NodeToTaskMapping OnlineSimulatedAnnealingWithMigrationsAndPreemptions::getNewSolutionFromNeighbourhood(
    const NodeToTaskMapping &solution)
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

} // namespace simulatedannealing
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
