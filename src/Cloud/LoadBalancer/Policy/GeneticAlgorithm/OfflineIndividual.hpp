#pragma once

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"
#include "Individual.hpp"
#include "Utility/RandomNumberGenerator.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

class OfflineIndividual : public Individual
{
  public:
    OfflineIndividual();

    OfflineIndividual(const NodeToTaskMapping &solution, const InfrastructureCPtr &infrastructure,
                      const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor,
                      const utility::RandomNumberGeneratorPtr randomNumberGenerator);

    IndividualPtr crossover(const IndividualPtr &rightParent) override;

    std::optional<double> mutate(const double probability) override;

  private:
    OfflineIndividual(const NodeToTaskMapping &parentSolution, const std::vector<Gene> &chromosome,
                      const InfrastructureCPtr &infrastructure,
                      const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor,
                      const utility::RandomNumberGeneratorPtr randomNumberGenerator);

    std::vector<Gene> repairChromosome(const std::vector<Gene> &chromosome);
};

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
