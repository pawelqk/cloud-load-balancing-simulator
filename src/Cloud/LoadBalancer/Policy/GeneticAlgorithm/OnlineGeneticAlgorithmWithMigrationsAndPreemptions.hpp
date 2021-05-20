#pragma once

#include <cstdint>
#include <list>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"
#include "Cloud/Task.hpp"
#include "GeneticAlgorithmBase.hpp"
#include "Utility/RandomNumberGenerator.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

class OnlineGeneticAlgorithmWithMigrationsAndPreemptions : public GeneticAlgorithmBase
{
  public:
    OnlineGeneticAlgorithmWithMigrationsAndPreemptions(const InfrastructureCPtr &infrastructure,
                                                       const Parameters &parameters,
                                                       const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor,
                                                       const logger::LoggerPtr &logger,
                                                       const utility::RandomNumberGeneratorPtr &randomNumberGenerator);

    std::string toString() const override;

  private:
    NodeToTaskMapping buildNodeToTaskMappingInternal(const TaskPtrVec &tasks) override;
};

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
