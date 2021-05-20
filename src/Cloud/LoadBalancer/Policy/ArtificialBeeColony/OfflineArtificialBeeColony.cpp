#include "OfflineArtificialBeeColony.hpp"

#include <utility>

#include "Cloud/TaskImpl.hpp"
#include "Utility/RandomNumberGenerator.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace artificialbeecolony
{

OfflineArtificialBeeColony::OfflineArtificialBeeColony(const InfrastructureCPtr &infrastructure,
                                                       const Parameters &parameters,
                                                       mapping::MappingAssessorPtr &&mappingAssessor,
                                                       const configuration::Instance &instance,
                                                       const logger::LoggerPtr &logger, const double penaltyFactor)
    : ArtificialBeeColonyBase(infrastructure, parameters, std::move(mappingAssessor), logger), instance(instance),
      penaltyFactor(penaltyFactor), offlineProblemAdapter(infrastructure, buildSolution())
{
}

NodeToTaskMapping OfflineArtificialBeeColony::buildNodeToTaskMappingInternal(const TaskPtrVec &tasks)
{
    return offlineProblemAdapter.getNextMapping(tasks);
}

std::string OfflineArtificialBeeColony::toString() const
{
    return "OfflineArtificialBeeColony";
}

NodeToTaskMapping OfflineArtificialBeeColony::buildSolution()
{
    const auto &taskDatasOverTime = instance.getTasks();

    TaskPtrVec tasks;
    tasks.reserve(taskDatasOverTime.size());
    for (auto &&[timePoint, taskDatas] : taskDatasOverTime)
    {
        for (auto &&taskData : taskDatas)
            tasks.push_back(std::make_shared<TaskImpl>(taskData.id, taskData.requiredMips, taskData.length, timePoint,
                                                       penaltyFactor));
    }

    auto solution = createNewSolution(tasks);
    logger->debug("whole solution: %s", ::cloud::loadbalancer::policy::toString(solution).c_str());

    return solution;
}

NodeToTaskMapping OfflineArtificialBeeColony::createRandomSolution(const TaskPtrVec &tasks)
{
    logger->debug("Creating random solution");
    NodeToTaskMapping solution;

    auto tasksShuffled = tasks;
    std::shuffle(tasksShuffled.begin(), tasksShuffled.end(), utility::RandomNumberGenerator::getInstance());

    std::stable_sort(tasksShuffled.begin(), tasksShuffled.end(), [](auto &&leftTask, auto &&rightTask) {
        return leftTask->getArrivalTime() < rightTask->getArrivalTime();
    });

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

    logger->debug("random solution: %s", ::cloud::loadbalancer::policy::toString(solution).c_str());
    return solution;
}

NodeToTaskMapping OfflineArtificialBeeColony::getNewSolutionFromNeighbourhood(const NodeToTaskMapping &solution)
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

    const auto randomDestinationNodeBegin =
        std::find_if(randomDestinationNode.begin(), randomDestinationNode.end(), [movedElement](auto &&task) {
            return movedElement->getArrivalTime() == task->getArrivalTime() ||
                   movedElement->getArrivalTime() < task->getArrivalTime();
        });
    const auto randomDestinationNodeEnd =
        std::find_if(randomDestinationNode.begin(), randomDestinationNode.end(),
                     [movedElement](auto &&task) { return movedElement->getArrivalTime() < task->getArrivalTime(); });

    const auto movedElementDestinationIt =
        randomDestinationNode.empty()
            ? randomDestinationNode.begin()
            : std::next(randomDestinationNodeBegin,
                        std::uniform_int_distribution<>(
                            0, std::distance(randomDestinationNodeBegin, randomDestinationNodeEnd))(rng));

    randomDestinationNode.insert(movedElementDestinationIt, movedElement);

    return solutionInNeighbourhood;
}

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
