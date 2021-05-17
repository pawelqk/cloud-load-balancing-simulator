#include "OfflineSimulatedAnnealing.hpp"

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

OfflineSimulatedAnnealing::OfflineSimulatedAnnealing(const InfrastructureCPtr &infrastructure,
                                                     const Parameters &parameters,
                                                     mapping::MappingAssessorPtr &&mappingAssessor,
                                                     const configuration::Instance &instance,
                                                     const logger::LoggerPtr &logger)
    : SimulatedAnnealingBase(infrastructure, parameters, std::move(mappingAssessor), logger), instance(instance)
{
}

NodeToTaskMapping OfflineSimulatedAnnealing::buildNodeToTaskMappingInternal(const TaskPtrVec &tasks)
{
    logger->debug("Mapping %u tasks", tasks.size());

    const auto solution = createNewSolution(tasks);

    return solution;
}

std::string OfflineSimulatedAnnealing::toString() const
{
    return "OfflineSimulatedAnnealing";
}

NodeToTaskMapping OfflineSimulatedAnnealing::createRandomSolution(const TaskPtrVec &tasks)
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

NodeToTaskMapping OfflineSimulatedAnnealing::getNewSolutionFromNeighbourhood(const NodeToTaskMapping &solution)
{
    // neighbourhood of type INSERT
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

} // namespace simulatedannealing
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
