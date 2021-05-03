#pragma once

#include <functional>
#include <list>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"
#include "Cloud/Task.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace simulatedannealing
{

struct Parameters
{
    double coolingRatio;
    double startTemperature;
    double endTemperature;
    std::uint16_t iterationsPerStep;
};

class SimulatedAnnealing : public PolicyBase
{
  public:
    SimulatedAnnealing(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                       mapping::MappingAssessorPtr &&mappingAssessor, const logger::LoggerPtr &logger);

    NodeToTaskMapping buildNodeToTaskMapping(const TaskPtrVec &tasks) override;

    std::string toString() const override;

  private:
    NodeToTaskMapping createNewSolution(const TaskPtrVec &tasks);
    NodeToTaskMapping createRandomSolution(const TaskPtrVec &tasks);
    NodeToTaskMapping getNewSolutionFromNeighbourhood(const NodeToTaskMapping &solution);
    std::vector<NodeId> extractFreeNodeIds();

    const Parameters parameters;
    const mapping::MappingAssessorPtr mappingAssessor;

    const logger::LoggerPtr logger;
};

} // namespace simulatedannealing
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
