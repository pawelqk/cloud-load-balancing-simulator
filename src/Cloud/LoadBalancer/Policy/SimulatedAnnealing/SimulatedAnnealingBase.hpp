#pragma once

#include <cstdint>
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
    // std::uint16_t maxIterationsWithoutChange;
};

class SimulatedAnnealingBase : public PolicyBase
{
  public:
    SimulatedAnnealingBase(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                           mapping::MappingAssessorPtr &&mappingAssessor, const logger::LoggerPtr &logger);

  protected:
    NodeToTaskMapping createNewSolution(const TaskPtrVec &tasks);
    virtual NodeToTaskMapping createRandomSolution(const TaskPtrVec &tasks) = 0;
    virtual NodeToTaskMapping getNewSolutionFromNeighbourhood(const NodeToTaskMapping &solution) = 0;
    NodeToTaskMapping tweakSolution(const NodeToTaskMapping &solution);
    std::vector<NodeId> findNotEmptyNodeIds(const NodeToTaskMapping &solution);
    std::vector<NodeId> findFeasibleNodeIds(const NodeToTaskMapping &solution, const TaskPtr &task);
    std::vector<NodeId> extractFreeNodeIds();

    const Parameters parameters;
    const mapping::MappingAssessorPtr mappingAssessor;
    std::uint32_t iterationsWithoutChange;
};

} // namespace simulatedannealing
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
