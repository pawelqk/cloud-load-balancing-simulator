#pragma once

#include <cstdint>
#include <list>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"
#include "Cloud/Task.hpp"
#include "GeneticAlgorithmBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

class OnlineGeneticAlgorithmWithMigrationsAndPreemptions : public PolicyBase
{
  public:
    OnlineGeneticAlgorithmWithMigrationsAndPreemptions(const InfrastructureCPtr &infrastructure,
                                                       const Parameters &parameters,
                                                       mapping::MappingAssessorPtr &&mappingAssessor,
                                                       const logger::LoggerPtr &logger);

    NodeToTaskMapping buildNodeToTaskMapping(const TaskPtrVec &tasks) override;

    std::string toString() const override;

  protected:
    const Parameters parameters;
    const mapping::MappingAssessorPtr mappingAssessor;
};

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
