#pragma once

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

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

    Individual(const NodeToTaskMapping &solution, const InfrastructureCPtr &infrastructure,
               const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor);

    static Individual crossover(const Individual &leftParent, const Individual &rightParent);

    std::optional<double> mutate(const double probability);

    const NodeToTaskMapping &getSolution() const;
    double getFitnessValue() const;

    // for testing purposes
    const std::vector<Gene> &getChromosome() const;

  private:
    Individual(const NodeToTaskMapping &parentSolution, const std::vector<Gene> &chromosome,
               const InfrastructureCPtr &infrastructure,
               const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor);

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
};

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
