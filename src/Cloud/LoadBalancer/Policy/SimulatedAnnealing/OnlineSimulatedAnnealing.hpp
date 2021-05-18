#pragma once

#include <functional>
#include <list>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/Task.hpp"
#include "SimulatedAnnealingBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace simulatedannealing
{

class OnlineSimulatedAnnealing : public SimulatedAnnealingBase
{
  public:
    OnlineSimulatedAnnealing(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                             mapping::MappingAssessorPtr &&mappingAssessor, const logger::LoggerPtr &logger);

    std::string toString() const override;

  private:
    NodeToTaskMapping buildNodeToTaskMappingInternal(const TaskPtrVec &tasks);

    NodeToTaskMapping createInitialSolution(const TaskPtrVec &tasks) override;
    NodeToTaskMapping getNewSolutionFromNeighbourhood(const NodeToTaskMapping &solution) override;
    NodeToTaskMapping adjustSolutionWithExistingTasks(const NodeToTaskMapping &solution);
};

} // namespace simulatedannealing
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
