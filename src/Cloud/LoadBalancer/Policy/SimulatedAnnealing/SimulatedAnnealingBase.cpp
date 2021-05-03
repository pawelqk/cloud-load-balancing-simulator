#include "SimulatedAnnealingBase.hpp"

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

SimulatedAnnealingBase::SimulatedAnnealingBase(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                                               mapping::MappingAssessorPtr &&mappingAssessor,
                                               const logger::LoggerPtr &logger)
    : PolicyBase(infrastructure, logger), parameters(parameters), mappingAssessor(std::move(mappingAssessor)),
      iterationsWithoutChange(0)
{
}

NodeToTaskMapping SimulatedAnnealingBase::createNewSolution(const TaskPtrVec &tasks)
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
    bool changedSolution{false};
    while (temperature > parameters.endTemperature)
    {
        nextSolution = getNewSolutionFromNeighbourhood(currentSolution);
        const auto nextSolutionValue = mappingAssessor->assess(nextSolution);
        const auto currentSolutionValue = mappingAssessor->assess(currentSolution);

        if (nextSolutionValue <= currentSolutionValue)
        {
            currentSolution = nextSolution;
            bestSolution = nextSolution;
            changedSolution = true;
        }
        else
        {
            changedSolution = false;
            if (distribution(rng) < std::exp((currentSolutionValue - nextSolutionValue) / temperature))
                currentSolution = nextSolution;
        }

        if (!changedSolution)
            ++iterationsWithoutChange;
        else
            iterationsWithoutChange = 0;

        // if (iterationsWithoutChange == 10000)
        // currentSolution = tweakSolution(currentSolution);
        // break;

        if ((++numberOfIterations) % parameters.iterationsPerStep == 0)
            temperature *= parameters.coolingRatio;
    }

    return bestSolution;
}

NodeToTaskMapping SimulatedAnnealingBase::tweakSolution(const NodeToTaskMapping &solution)
{
    return solution;
}

std::vector<NodeId> SimulatedAnnealingBase::extractFreeNodeIds()
{
    std::vector<NodeId> freeNodeIds;
    for (auto &&node : infrastructure->getNodes())
    {
        if (node->isIdle())
            freeNodeIds.push_back(node->getId());
    }

    return freeNodeIds;
}

std::vector<NodeId> SimulatedAnnealingBase::findNotEmptyNodeIds(const NodeToTaskMapping &solution)
{
    std::vector<NodeId> notEmptyNodeIds;
    for (auto &&entry : solution)
    {
        if (!entry.second.empty())
            notEmptyNodeIds.push_back(entry.first);
    }

    return notEmptyNodeIds;
}

std::vector<NodeId> SimulatedAnnealingBase::findFeasibleNodeIds(const NodeToTaskMapping &solution, const TaskPtr &task)
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

} // namespace simulatedannealing
} // namespace policy
} // namespace loadbalancer
} // namespace cloud