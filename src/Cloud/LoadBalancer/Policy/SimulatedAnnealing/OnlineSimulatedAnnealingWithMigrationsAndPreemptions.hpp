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

class OnlineSimulatedAnnealingWithMigrationsAndPreemptions : public SimulatedAnnealingBase
{
  public:
    OnlineSimulatedAnnealingWithMigrationsAndPreemptions(const InfrastructureCPtr &infrastructure,
                                                         const Parameters &parameters,
                                                         mapping::MappingAssessorPtr &&mappingAssessor,
                                                         const logger::LoggerPtr &logger);

    NodeToTaskMapping buildNodeToTaskMappingInternal(const TaskPtrVec &tasks) override;

    std::string toString() const override;

  private:
    NodeToTaskMapping createRandomSolution(const TaskPtrVec &tasks) override;
    NodeToTaskMapping getNewSolutionFromNeighbourhood(const NodeToTaskMapping &solution) override;
};

} // namespace simulatedannealing
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
