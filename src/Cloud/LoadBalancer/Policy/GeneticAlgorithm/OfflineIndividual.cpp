#include "OfflineIndividual.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

namespace
{
using OfflineIndividualPtr = std::shared_ptr<OfflineIndividual>;
}

OfflineIndividual::OfflineIndividual() = default;

OfflineIndividual::OfflineIndividual(const NodeToTaskMapping &solution, const InfrastructureCPtr &infrastructure,
                                     const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor,
                                     const utility::RandomNumberGeneratorPtr randomNumberGenerator)
    : Individual(solution, infrastructure, mappingAssessor, randomNumberGenerator)
{
}

OfflineIndividual::OfflineIndividual(const NodeToTaskMapping &parentSolution, const std::vector<Gene> &chromosome,
                                     const InfrastructureCPtr &infrastructure,
                                     const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor,
                                     const utility::RandomNumberGeneratorPtr randomNumberGenerator)
    : Individual(parentSolution, chromosome, infrastructure, mappingAssessor, randomNumberGenerator)
{
}

IndividualPtr OfflineIndividual::crossover(const IndividualPtr &rightParent)
{
    // Order Crossover operator (OX)
    // https://mat.uab.cat/~alseda/MasterOpt/GeneticOperations.pdf

    auto [beginIndex, endIndex] = getRandomChromosomeSlice();
    std::vector<Gene> offspring(chromosome.size());

    std::set<std::uint32_t> insertedGenes;
    for (auto i = beginIndex; i <= endIndex; ++i)
    {
        offspring[i] = chromosome[i];
        insertedGenes.insert(offspring[i].taskId);
    }

    auto offspringIndex = (endIndex + 1) % offspring.size();
    for (auto i = (endIndex + 1) % offspring.size(); offspringIndex != beginIndex; i = (i + 1) % offspring.size())
    {
        if (!insertedGenes.contains(rightParent->getChromosome()[i].taskId))
        {
            offspring[offspringIndex] = rightParent->getChromosome()[i];
            offspringIndex = (offspringIndex + 1) % offspring.size();
        }
    }

    return OfflineIndividualPtr(new OfflineIndividual(solution, repairChromosome(offspring), infrastructure,
                                                      mappingAssessor, randomNumberGenerator));
}

std::optional<double> OfflineIndividual::mutate(const double probability)
{
    std::uniform_real_distribution<> dist(0, 1.0);

    bool mutated = false;
    auto newChromosome = chromosome;

    for (auto i = 0u; i < chromosome.size(); ++i)
    {
        const auto &taskId = chromosome[i].taskId;
        const auto &nodeId = chromosome[i].nodeId;
        if (dist(*randomNumberGenerator) <= probability)
        {
            const auto task = getTask(taskId, nodeId);
            const auto feasibleNodeIds = infrastructure->findFeasibleNodeIds(task);

            std::uniform_int_distribution<> intDist(0, feasibleNodeIds.size() - 1);
            const auto newNodeId = feasibleNodeIds[intDist(*randomNumberGenerator)];

            if (newNodeId != nodeId)
            {
                newChromosome[i].nodeId = newNodeId;
                mutated = true;
            }
        }
    }

    if (mutated)
    {
        chromosome = repairChromosome(newChromosome);
        decode();
        fitnessValue = mappingAssessor->assess(solution);
        return fitnessValue;
    }

    return std::nullopt;
}

std::vector<Individual::Gene> OfflineIndividual::repairChromosome(const std::vector<Gene> &chromosome)
{
    std::map<NodeId, std::vector<TaskPtr>> sortableMapping;
    for (auto &[taskId, nodeId] : chromosome)
        sortableMapping[nodeId].push_back(getTask(taskId));

    for (auto &[nodeId, tasks] : sortableMapping)
    {
        std::stable_sort(tasks.begin(), tasks.end(), [](auto &&leftTask, auto &&rightTask) {
            return leftTask->getArrivalTime() < rightTask->getArrivalTime();
        });
    }

    std::vector<Gene> repairedChromosome;
    for (auto &&[nodeId, tasks] : sortableMapping)
    {
        for (auto &&task : tasks)
            repairedChromosome.emplace_back(task->getId(), nodeId);
    }

    return repairedChromosome;
}

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
