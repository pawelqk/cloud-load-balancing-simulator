#pragma once

#include <functional>
#include <list>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/Task.hpp"
#include "Policy.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

// TODO: extract
using Solution = std::map<NodeId, std::list<Task>>;
using SolutionAssessor = std::function<double(Solution)>;

class SimulatedAnnealing : public Policy
{
  public:
    struct Parameters
    {
        double coolingRatio;
        double startTemperature;
        double endTemperature;
        std::uint16_t iterationsPerStep;
        // SolutionAssessor solutionAssessor;
    };

    SimulatedAnnealing(const InfrastructureCPtr &infrastructure, const Parameters &parameters);

    MappingActions buildTaskToNodeMapping(const TaskSet &tasks) override;

  private:
    Solution createNewSolution(const TaskSet &tasks);
    Solution createRandomSolution(const TaskSet &tasks);
    Solution getNewSolutionFromNeighbourhood(const Solution &solution);
    std::vector<NodeId> extractFreeNodeIds();

    const Parameters parameters;

    Solution solution;
    TaskSet waitingTasks;

    logger::Logger logger;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
