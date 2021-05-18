#pragma once

#include <functional>
#include <list>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/OfflineProblemAdapter.hpp"
#include "Cloud/Task.hpp"
#include "Configuration/Instance.hpp"
#include "SimulatedAnnealingBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace simulatedannealing
{

class OfflineSimulatedAnnealing : public SimulatedAnnealingBase
{
  public:
    OfflineSimulatedAnnealing(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                              mapping::MappingAssessorPtr &&mappingAssessor, const configuration::Instance &instance,
                              const logger::LoggerPtr &logger, const double penaltyFactor);

    NodeToTaskMapping buildNodeToTaskMappingInternal(const TaskPtrVec &tasks) override;

    std::string toString() const override;

  private:
    NodeToTaskMapping buildSolution();
    NodeToTaskMapping createInitialSolution(const TaskPtrVec &tasks) override;
    NodeToTaskMapping getNewSolutionFromNeighbourhood(const NodeToTaskMapping &solution) override;

    const configuration::Instance instance;
    const double penaltyFactor;

    OfflineProblemAdapter offlineProblemAdapter;
};

} // namespace simulatedannealing
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
