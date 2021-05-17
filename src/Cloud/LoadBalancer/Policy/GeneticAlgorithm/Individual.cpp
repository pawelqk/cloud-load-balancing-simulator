#include "Individual.hpp"

#include <iostream>
#include <optional>
#include <random>
#include <utility>

#include "Utility/RandomNumberGenerator.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

bool Individual::Gene::operator==(const Gene &other) const
{
    return taskId == other.taskId && nodeId == other.nodeId;
}

Individual::Individual() = default;

Individual::Individual(const NodeToTaskMapping &solution, const InfrastructureCPtr &infrastructure,
                       const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor)
    : solution(solution), infrastructure(infrastructure), mappingAssessor(mappingAssessor),
      fitnessValue(mappingAssessor->assess(solution))
{
    encode();
}

Individual::Individual(const NodeToTaskMapping &parentSolution, const std::vector<Gene> &chromosome,
                       const InfrastructureCPtr &infrastructure,
                       const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor)
    : solution(parentSolution), chromosome(chromosome), infrastructure(infrastructure), mappingAssessor(mappingAssessor)
{
    decode();
    fitnessValue = mappingAssessor->assess(solution);
}

Individual Individual::crossover(const Individual &leftParent, const Individual &rightParent)
{
    // Order Crossover operator (OX)
    // https://mat.uab.cat/~alseda/MasterOpt/GeneticOperations.pdf

    auto [beginIndex, endIndex] = leftParent.getRandomChromosomeSlice();
    std::vector<Gene> offspring(leftParent.chromosome.size());

    std::set<std::uint32_t> insertedGenes;
    for (auto i = beginIndex; i <= endIndex; ++i)
    {
        offspring[i] = leftParent.chromosome[i];
        insertedGenes.insert(offspring[i].taskId);
    }

    auto offspringIndex = (endIndex + 1) % offspring.size();
    for (auto i = (endIndex + 1) % offspring.size(); offspringIndex != beginIndex; i = (i + 1) % offspring.size())
    {
        if (!insertedGenes.contains(rightParent.chromosome[i].taskId))
        {
            offspring[offspringIndex] = rightParent.chromosome[i];
            offspringIndex = (offspringIndex + 1) % offspring.size();
        }
    }

    return Individual{leftParent.solution, offspring, leftParent.infrastructure, leftParent.mappingAssessor};
}

std::optional<double> Individual::mutate(const double probability)
{
    auto &rng = utility::RandomNumberGenerator::getInstance();
    std::uniform_real_distribution<> dist(0, 1.0);

    bool mutated = false;
    auto newChromosome = chromosome;

    for (auto i = 0u; i < chromosome.size(); ++i)
    {
        const auto &taskId = chromosome[i].taskId;
        const auto &nodeId = chromosome[i].nodeId;
        if (dist(rng) <= probability)
        {
            const auto task = getTask(taskId, nodeId);
            const auto feasibleNodeIds = infrastructure->findFeasibleNodeIds(task);

            std::uniform_int_distribution<> intDist(0, feasibleNodeIds.size() - 1);
            const auto newNodeId = feasibleNodeIds[intDist(rng)];

            if (newNodeId != nodeId)
            {
                newChromosome[i].nodeId = newNodeId;
                mutated = true;
            }
        }
    }

    if (mutated)
    {
        chromosome = std::move(newChromosome);
        decode();
        fitnessValue = mappingAssessor->assess(solution);
        return fitnessValue;
    }

    return std::nullopt;
}

const NodeToTaskMapping &Individual::getSolution() const
{
    return solution;
}

double Individual::getFitnessValue() const
{
    return fitnessValue;
}

const std::vector<Individual::Gene> &Individual::getChromosome() const
{
    return chromosome;
}

void Individual::encode()
{
    chromosome.clear();
    for (auto &&[nodeId, tasks] : solution)
    {
        for (auto &&task : tasks)
            chromosome.emplace_back(task->getId(), nodeId);
    }
}

std::pair<std::uint32_t, std::uint32_t> Individual::getRandomChromosomeSlice() const
{
    auto &rng = utility::RandomNumberGenerator::getInstance();

    std::uniform_int_distribution<> firstDist(0, chromosome.size() - 1);
    std::uint32_t begin = firstDist(rng);

    std::uniform_int_distribution<> secondDist(begin, chromosome.size() - 1);
    std::uint32_t end = secondDist(rng);

    return std::make_pair(begin, end);
}

TaskPtr Individual::getTask(const std::uint32_t taskId, const NodeId nodeId)
{
    const auto &nodeTasks = solution[nodeId];
    const auto taskIt =
        std::find_if(nodeTasks.cbegin(), nodeTasks.cend(), [taskId](auto &&task) { return task->getId() == taskId; });

    if (taskIt == nodeTasks.cend())
        throw std::runtime_error("Unable to find task in given node");

    return *taskIt;
}

TaskPtr Individual::getTask(const std::uint32_t taskId)
{
    std::optional<TaskPtr> task;

    for (auto &&[_, tasks] : solution)
    {
        const auto taskIt =
            std::find_if(tasks.cbegin(), tasks.cend(), [taskId](auto &&task) { return task->getId() == taskId; });

        if (taskIt != tasks.cend())
            task = *taskIt;
    }

    if (task == std::nullopt)
        throw std::runtime_error("Unable to find task in current solution");

    return *task;
}

void Individual::decode()
{
    NodeToTaskMapping newSolution;
    for (auto &[taskId, nodeId] : chromosome)
        newSolution[nodeId].push_back(getTask(taskId));

    solution = std::move(newSolution);
}

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
