#pragma once

#include <cstdint>
#include <list>

#include "ArtificialBeeColonyBase.hpp"
#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/OfflineProblemAdapter.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"
#include "Cloud/Task.hpp"
#include "Configuration/Instance.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace artificialbeecolony
{

class OfflineArtificialBeeColony : public ArtificialBeeColonyBase
{
  public:
    OfflineArtificialBeeColony(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                               mapping::MappingAssessorPtr &&mappingAssessor,
                               const utility::RandomNumberGeneratorPtr &randomNumberGenerator,
                               const configuration::Instance &instance, const logger::LoggerPtr &logger,
                               const double penaltyFactor);

    NodeToTaskMapping buildNodeToTaskMappingInternal(const TaskPtrVec &tasks) override;

    std::string toString() const override;

  private:
    NodeToTaskMapping buildSolution();
    NodeToTaskMapping createRandomSolution(const TaskPtrVec &tasks) override;
    NodeToTaskMapping getNewSolutionFromNeighbourhood(const NodeToTaskMapping &solution) override;

    const configuration::Instance instance;
    const double penaltyFactor;

    OfflineProblemAdapter offlineProblemAdapter;
};

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
