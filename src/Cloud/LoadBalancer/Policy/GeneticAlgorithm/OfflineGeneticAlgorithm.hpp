#pragma once

#include <cstdint>
#include <list>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/OfflineProblemAdapter.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"
#include "Cloud/Task.hpp"
#include "Configuration/Instance.hpp"
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

class OfflineGeneticAlgorithm : public GeneticAlgorithmBase
{
  public:
    OfflineGeneticAlgorithm(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                            const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor,
                            const configuration::Instance &instance, const logger::LoggerPtr &logger,
                            const utility::RandomNumberGeneratorPtr &randomNumberGenerator, const double penaltyFactor);

    NodeToTaskMapping buildNodeToTaskMappingInternal(const TaskPtrVec &tasks) override;

    std::string toString() const override;

  private:
    NodeToTaskMapping buildSolution();
    IndividualPtr generateRandomIndividual(const TaskPtrVec &tasks) override;

    const configuration::Instance instance;
    const double penaltyFactor;

    OfflineProblemAdapter offlineProblemAdapter;
};

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
