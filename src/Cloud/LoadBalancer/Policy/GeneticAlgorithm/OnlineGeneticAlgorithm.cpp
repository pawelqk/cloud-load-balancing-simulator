#include "OnlineGeneticAlgorithm.hpp"

#include <cmath>
#include <iterator>
#include <optional>
#include <random>

#include "Utility/RandomNumberGenerator.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

OnlineGeneticAlgorithm::OnlineGeneticAlgorithm(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                                               const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor,
                                               const logger::LoggerPtr &logger)
    : PolicyBase(infrastructure, logger), parameters(parameters), mappingAssessor(mappingAssessor)
{
    population.reserve(parameters.populationSize);
}

std::string OnlineGeneticAlgorithm::toString() const
{
    return "OnlineGeneticAlgorithm";
}

NodeToTaskMapping OnlineGeneticAlgorithm::buildNodeToTaskMappingInternal(const TaskPtrVec &tasks)
{
    logger->debug("Mapping %u tasks", tasks.size());

    population.clear();

    return adjustSolutionWithExistingTasks(createNewSolution(tasks));
}

NodeToTaskMapping OnlineGeneticAlgorithm::createNewSolution(const TaskPtrVec &tasks)
{
    auto bestSolution = generateInitialPopulation(tasks);
    for (auto iteration = 0u; iteration < parameters.maxIterations; ++iteration)
    {
        const auto parentPairs = getParentPairs();
        auto descendants = performCrossover(parentPairs);
        for (auto &descendant : descendants)
        {
            const auto descendantValue = descendant.getFitnessValue();
            if (descendantValue < bestSolution.value)
            {
                bestSolution.solution = descendant.getSolution();
                bestSolution.value = descendantValue;
            }

            const auto newFitnessValue = descendant.mutate(parameters.mutationProbability);
            if (newFitnessValue != std::nullopt && *newFitnessValue < bestSolution.value)
            {
                bestSolution.solution = descendant.getSolution();
                bestSolution.value = *newFitnessValue;
            }
        }

        chooseNextPopulation(descendants);
    }

    return bestSolution.solution;
}

NodeToTaskMapping OnlineGeneticAlgorithm::adjustSolutionWithExistingTasks(const NodeToTaskMapping &solution)
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

OnlineGeneticAlgorithm::BestSolution OnlineGeneticAlgorithm::generateInitialPopulation(const TaskPtrVec &tasks)
{
    const auto &firstIndividual = population.emplace_back(generateRandomIndividual(tasks));
    auto bestSolution = firstIndividual.getSolution();
    auto bestValue = mappingAssessor->assess(bestSolution);

    for (auto i = 1u; i < parameters.populationSize; ++i)
    {
        const auto &individual = population.emplace_back(generateRandomIndividual(tasks));
        const auto &currentSolution = individual.getSolution();
        const auto currentValue = mappingAssessor->assess(currentSolution);
        if (currentValue < bestValue)
        {
            bestSolution = currentSolution;
            bestValue = currentValue;
        }
    }

    return {bestSolution, bestValue};
}

Individual OnlineGeneticAlgorithm::generateRandomIndividual(const TaskPtrVec &tasks)
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

    return Individual{solution, infrastructure, mappingAssessor};
}

std::vector<OnlineGeneticAlgorithm::ParentPair> OnlineGeneticAlgorithm::getParentPairs()
{
    auto breedingSubpopulation = selectWithRouletteWheel(population, population.size());
    std::shuffle(breedingSubpopulation.begin(), breedingSubpopulation.end(),
                 utility::RandomNumberGenerator::getInstance());

    std::vector<ParentPair> parentPairs;
    for (auto i = 0u; i < breedingSubpopulation.size() - 1; i += 2)
        parentPairs.emplace_back(breedingSubpopulation[i], breedingSubpopulation[i + 1]);

    return parentPairs;
}

std::vector<Individual> OnlineGeneticAlgorithm::performCrossover(const std::vector<ParentPair> &parentPairs)
{
    std::vector<Individual> descendants;
    descendants.reserve(parentPairs.size());

    for (auto &&parentPair : parentPairs)
        descendants.emplace_back(Individual::crossover(parentPair.left, parentPair.right));

    return descendants;
}

void OnlineGeneticAlgorithm::chooseNextPopulation(const std::vector<Individual> &descendants)
{
    auto bothGenerations = population;
    std::copy(descendants.begin(), descendants.end(), std::back_inserter(bothGenerations));

    std::sort(bothGenerations.begin(), bothGenerations.end(), [](auto &&leftIndividual, auto &&rightIndividual) {
        return leftIndividual.getFitnessValue() > rightIndividual.getFitnessValue();
    });

    const auto elitesCount = std::ceil(bothGenerations.size() * parameters.eliteIndividualsRatio);

    std::vector<Individual> eliteIndividuals(bothGenerations.rbegin(),
                                             std::next(bothGenerations.rbegin(), elitesCount));

    bothGenerations.resize(bothGenerations.size() - elitesCount);

    // TODO: isnt it bad that we can have more than once single individual?
    auto nextPopulation = selectWithRouletteWheel(bothGenerations, parameters.populationSize - elitesCount);

    std::copy(eliteIndividuals.begin(), eliteIndividuals.end(), std::back_inserter(nextPopulation));

    population = std::move(nextPopulation);
}

std::vector<Individual> OnlineGeneticAlgorithm::selectWithRouletteWheel(const std::vector<Individual> &individuals,
                                                                        const std::vector<Individual>::size_type count)
{
    std::vector<double> weights(individuals.size());
    for (auto i = 0u; i < individuals.size(); ++i)
        weights[i] = 1.0 / individuals[i].getFitnessValue();

    auto &rng = utility::RandomNumberGenerator::getInstance();
    std::discrete_distribution dist(weights.cbegin(), weights.cend());

    std::vector<Individual> chosenIndividuals(count);
    for (auto i = 0u; i < count; ++i)
        chosenIndividuals[i] = individuals[dist(rng)];

    return chosenIndividuals;
}

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
