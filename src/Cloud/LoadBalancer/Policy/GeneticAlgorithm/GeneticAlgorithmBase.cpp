
#include "GeneticAlgorithmBase.hpp"

#include <cmath>
#include <iterator>
#include <optional>
#include <random>
#include <sstream>

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
constexpr auto POPULATION_TO_SELECT_ELITE_MULTIPLIER = 100;
};

std::string toString(const Parameters &parameters)
{
    std::stringstream ss;
    ss << parameters.mutationProbability << "_" << parameters.populationSize << "_" << parameters.maxIterations << "_"
       << parameters.eliteIndividualsInInitialGenerationRatio << "_"
       << parameters.eliteIndividualsInNextGenerationRatio;

    return ss.str();
}

GeneticAlgorithmBase::GeneticAlgorithmBase(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                                           const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor,
                                           const logger::LoggerPtr &logger,
                                           const utility::RandomNumberGeneratorPtr &randomNumberGenerator)
    : PolicyBase(infrastructure, logger), mappingAssessor(mappingAssessor),
      randomNumberGenerator(randomNumberGenerator), parameters(parameters)
{
    population.reserve(parameters.populationSize);
}

NodeToTaskMapping GeneticAlgorithmBase::createNewSolution(const TaskPtrVec &tasks)
{
    population.clear();

    generateInitialPopulation(tasks);
    for (auto iteration = 0u; iteration < parameters.maxIterations; ++iteration)
    {
        const auto parentPairs = getParentPairs();
        auto descendants = performCrossover(parentPairs);
        for (auto &descendant : descendants)
        {
            const auto descendantValue = descendant->getFitnessValue();
            if (descendantValue < bestSolution.value)
            {
                bestSolution.solution = descendant->getSolution();
                bestSolution.value = descendantValue;
            }

            const auto newFitnessValue = descendant->mutate(parameters.mutationProbability);
            if (newFitnessValue != std::nullopt && *newFitnessValue < bestSolution.value)
            {
                bestSolution.solution = descendant->getSolution();
                bestSolution.value = *newFitnessValue;
            }
        }

        chooseNextPopulation(descendants);
    }

    return bestSolution.solution;
}

void GeneticAlgorithmBase::generateInitialPopulation(const TaskPtrVec &tasks)
{
    const std::uint32_t eliteIndividualsSize =
        std::ceil(parameters.eliteIndividualsInInitialGenerationRatio * parameters.populationSize);
    const std::uint32_t randomIndividualsSize = eliteIndividualsSize * POPULATION_TO_SELECT_ELITE_MULTIPLIER;

    std::vector<IndividualPtr> randomIndividuals(randomIndividualsSize);
    for (auto i = 0u; i < randomIndividualsSize; ++i)
        randomIndividuals[i] = generateRandomIndividual(tasks);

    const auto individualsSplitByElitism = splitIndividualsByElitism(randomIndividuals, eliteIndividualsSize);
    const auto &eliteIndividuals = individualsSplitByElitism.elite;

    const auto &firstIndividual = population.emplace_back(generateRandomIndividual(tasks));
    bestSolution.solution = firstIndividual->getSolution();
    bestSolution.value = mappingAssessor->assess(bestSolution.solution);

    for (auto i = 1u; i < parameters.populationSize - eliteIndividualsSize; ++i)
        insertNewIndividual(generateRandomIndividual(tasks));

    for (auto i = 0u; i < eliteIndividualsSize; ++i)
        insertNewIndividual(eliteIndividuals[i]);
}

IndividualPtr GeneticAlgorithmBase::generateRandomIndividual(const TaskPtrVec &tasks)
{
    NodeToTaskMapping solution;

    auto tasksShuffled = tasks;
    std::shuffle(tasksShuffled.begin(), tasksShuffled.end(), *randomNumberGenerator);

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

    return std::make_shared<Individual>(solution, infrastructure, mappingAssessor, randomNumberGenerator);
}

GeneticAlgorithmBase::IndividualsSplitByElitism GeneticAlgorithmBase::splitIndividualsByElitism(
    const std::vector<IndividualPtr> &individuals, const std::vector<IndividualPtr>::size_type count)
{
    if (count == 0)
        return {{}, individuals};

    auto notEliteIndividuals = individuals;

    std::sort(notEliteIndividuals.begin(), notEliteIndividuals.end(),
              [](auto &&leftIndividual, auto &&rightIndividual) {
                  return leftIndividual->getFitnessValue() > rightIndividual->getFitnessValue();
              });

    std::vector<IndividualPtr> eliteIndividuals(notEliteIndividuals.rbegin(),
                                                std::next(notEliteIndividuals.rbegin(), count));
    notEliteIndividuals.resize(notEliteIndividuals.size() - count);

    return {eliteIndividuals, notEliteIndividuals};
}

void GeneticAlgorithmBase::insertNewIndividual(const IndividualPtr &individual)
{
    const auto &insertedIndividual = population.emplace_back(individual);
    const auto &insertedSolution = insertedIndividual->getSolution();

    const auto insertedValue = mappingAssessor->assess(insertedSolution);
    if (insertedValue < bestSolution.value)
    {
        bestSolution.solution = insertedSolution;
        bestSolution.value = insertedValue;
    }
}

std::vector<GeneticAlgorithmBase::ParentPair> GeneticAlgorithmBase::getParentPairs()
{
    auto breedingSubpopulation = selectWithRouletteWheel(population, population.size());
    std::shuffle(breedingSubpopulation.begin(), breedingSubpopulation.end(), *randomNumberGenerator);

    std::vector<ParentPair> parentPairs;
    for (auto i = 0u; i < breedingSubpopulation.size() - 1; i += 2)
        parentPairs.emplace_back(breedingSubpopulation[i], breedingSubpopulation[i + 1]);

    return parentPairs;
}

std::vector<IndividualPtr> GeneticAlgorithmBase::performCrossover(const std::vector<ParentPair> &parentPairs)
{
    std::vector<IndividualPtr> descendants;
    descendants.reserve(parentPairs.size());

    for (auto &&parentPair : parentPairs)
        descendants.emplace_back(parentPair.left->crossover(parentPair.right));

    return descendants;
}

void GeneticAlgorithmBase::chooseNextPopulation(const std::vector<IndividualPtr> &descendants)
{
    auto bothGenerations = population;
    std::copy(descendants.begin(), descendants.end(), std::back_inserter(bothGenerations));

    const auto elitesCount = std::ceil(bothGenerations.size() * parameters.eliteIndividualsInNextGenerationRatio);

    const auto [eliteIndividuals, notEliteIndividuals] = splitIndividualsByElitism(bothGenerations, elitesCount);

    auto nextPopulation = selectUniqueWithRouletteWheel(notEliteIndividuals, parameters.populationSize - elitesCount);

    std::copy(eliteIndividuals.begin(), eliteIndividuals.end(), std::back_inserter(nextPopulation));

    population = std::move(nextPopulation);
}

std::vector<IndividualPtr> GeneticAlgorithmBase::selectWithRouletteWheel(
    const std::vector<IndividualPtr> &individuals, const std::vector<IndividualPtr>::size_type count)
{
    std::vector<double> weights(individuals.size());
    for (auto i = 0u; i < individuals.size(); ++i)
        weights[i] = 1.0 / individuals[i]->getFitnessValue();

    std::discrete_distribution dist(weights.cbegin(), weights.cend());

    std::vector<IndividualPtr> chosenIndividuals(count);
    for (auto i = 0u; i < count; ++i)
        chosenIndividuals[i] = individuals[dist(*randomNumberGenerator)];

    return chosenIndividuals;
}

std::vector<IndividualPtr> GeneticAlgorithmBase::selectUniqueWithRouletteWheel(
    const std::vector<IndividualPtr> &individuals, const std::vector<IndividualPtr>::size_type count)
{
    std::vector<double> weights(individuals.size());
    for (auto i = 0u; i < individuals.size(); ++i)
        weights[i] = 1.0 / individuals[i]->getFitnessValue();

    std::discrete_distribution dist(weights.cbegin(), weights.cend());

    std::set<std::vector<IndividualPtr>::size_type> chosenIds;
    std::vector<IndividualPtr> chosenIndividuals(count);
    for (auto i = 0u; i < count; ++i)
    {
        auto individualIdx = dist(*randomNumberGenerator);
        while (chosenIds.contains(individualIdx))
            individualIdx = dist(*randomNumberGenerator);

        chosenIndividuals[i] = individuals[individualIdx];
    }

    return chosenIndividuals;
}

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
