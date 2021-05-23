#pragma once

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"
#include "Utility/RandomNumberGenerator.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

class Individual;

using IndividualPtr = std::shared_ptr<Individual>;

class Individual
{
  public:
    struct Gene
    {
        std::uint32_t taskId;
        NodeId nodeId;

        bool operator==(const Gene &other) const;
    };

    Individual();

    virtual ~Individual();

    Individual(const NodeToTaskMapping &solution, const InfrastructureCPtr &infrastructure,
               const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor,
               const utility::RandomNumberGeneratorPtr randomNumberGenerator);

    virtual IndividualPtr crossover(const IndividualPtr &rightParent);

    virtual std::optional<double> mutate(const double probability);

    const NodeToTaskMapping &getSolution() const;
    double getFitnessValue() const;

    const std::vector<Gene> &getChromosome() const;

  protected:
    Individual(const NodeToTaskMapping &parentSolution, const std::vector<Gene> &chromosome,
               const InfrastructureCPtr &infrastructure,
               const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor,
               const utility::RandomNumberGeneratorPtr randomNumberGenerator);

    void encode();
    std::pair<std::uint32_t, std::uint32_t> getRandomChromosomeSlice() const;
    TaskPtr getTask(const std::uint32_t taskId, const NodeId nodeId);
    TaskPtr getTask(const std::uint32_t taskId);
    void decode();

    NodeToTaskMapping solution;
    std::vector<Gene> chromosome;
    InfrastructureCPtr infrastructure;
    std::shared_ptr<mapping::MappingAssessor> mappingAssessor;
    double fitnessValue;
    utility::RandomNumberGeneratorPtr randomNumberGenerator;
};

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
