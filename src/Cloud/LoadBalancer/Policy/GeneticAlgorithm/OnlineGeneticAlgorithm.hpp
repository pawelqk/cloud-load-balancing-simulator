#pragma once

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"
#include "Cloud/Task.hpp"
#include "GeneticAlgorithmBase.hpp"
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

class OnlineGeneticAlgorithm : public GeneticAlgorithmBase
{
  public:
    OnlineGeneticAlgorithm(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                           const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor,
                           const logger::LoggerPtr &logger,
                           const utility::RandomNumberGeneratorPtr &randomNumberGenerator);

    std::string toString() const override;

  private:
    NodeToTaskMapping buildNodeToTaskMappingInternal(const TaskPtrVec &tasks) override;
    NodeToTaskMapping adjustSolutionWithExistingTasks(const NodeToTaskMapping &solution);
};

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
