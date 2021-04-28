#pragma once

#include <functional>
#include <list>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/Task.hpp"
#include "PolicyBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

class SimulatedAnnealing : public PolicyBase
{
  public:
    struct Parameters
    {
        double coolingRatio;
        double startTemperature;
        double endTemperature;
        std::uint16_t iterationsPerStep;
        mapping::MappingAssessorPtr mappingAssessor;
    };

    SimulatedAnnealing(const InfrastructureCPtr &infrastructure, Parameters &&parameters,
                       const logger::LoggerPtr &logger);

    NodeToTaskMapping buildNodeToTaskMapping(const TaskPtrVec &tasks) override;

    std::string toString() const override;

  private:
    NodeToTaskMapping createNewSolution(const TaskPtrVec &tasks);
    NodeToTaskMapping createRandomSolution(const TaskPtrVec &tasks);
    NodeToTaskMapping getNewSolutionFromNeighbourhood(const NodeToTaskMapping &solution);
    std::vector<NodeId> extractFreeNodeIds();

    const Parameters parameters;

    const logger::LoggerPtr logger;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
