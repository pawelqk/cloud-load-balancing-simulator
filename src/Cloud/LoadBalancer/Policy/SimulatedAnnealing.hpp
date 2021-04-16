#pragma once

#include <functional>
#include <list>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/SolutionAssessor.hpp"
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
        SolutionAssessorPtr solutionAssessor;
    };

    SimulatedAnnealing(const InfrastructureCPtr &infrastructure, Parameters &&parameters,
                       const logger::LoggerPtr &logger);

    MappingActions buildTaskToNodeMapping(const TaskSet &tasks) override;

  private:
    Solution createNewSolution(const TaskSet &tasks);
    Solution createRandomSolution(const TaskSet &tasks);
    Solution getNewSolutionFromNeighbourhood(const Solution &solution);
    std::vector<NodeId> extractFreeNodeIds();

    const Parameters parameters;

    Solution solution;
    TaskSet waitingTasks;

    const logger::LoggerPtr logger;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
