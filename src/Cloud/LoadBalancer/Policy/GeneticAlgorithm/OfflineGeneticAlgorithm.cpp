#include "OfflineGeneticAlgorithm.hpp"
#include "Cloud/TaskImpl.hpp"

#include "Utility/RandomNumberGenerator.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

OfflineGeneticAlgorithm::OfflineGeneticAlgorithm(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                                                 const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor,
                                                 const configuration::Instance &instance,
                                                 const logger::LoggerPtr &logger,
                                                 const utility::RandomNumberGeneratorPtr &randomNumberGenerator,
                                                 const double penaltyFactor)
    : GeneticAlgorithmBase(infrastructure, parameters, mappingAssessor, logger, randomNumberGenerator),
      instance(instance), penaltyFactor(penaltyFactor), offlineProblemAdapter(infrastructure, buildSolution())

{
}

NodeToTaskMapping OfflineGeneticAlgorithm::buildNodeToTaskMappingInternal(const TaskPtrVec &tasks)
{
    return offlineProblemAdapter.getNextMapping(tasks);
}

NodeToTaskMapping OfflineGeneticAlgorithm::buildSolution()
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

Individual OfflineGeneticAlgorithm::generateRandomIndividual(const TaskPtrVec &tasks)
{
    NodeToTaskMapping solution;

    auto tasksShuffled = tasks;
    std::shuffle(tasksShuffled.begin(), tasksShuffled.end(), *randomNumberGenerator);

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
        solution[possibleNodeIds[dis(*randomNumberGenerator)]].push_back(task);
    }

    logger->debug("random solution: %s", ::cloud::loadbalancer::policy::toString(solution).c_str());
    return Individual{solution, infrastructure, mappingAssessor, randomNumberGenerator};
}

std::string OfflineGeneticAlgorithm::toString() const
{
    return "OfflineGeneticAlgorithm";
}

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
