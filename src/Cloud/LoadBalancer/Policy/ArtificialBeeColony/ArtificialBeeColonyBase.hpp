#pragma once

#include <cstdint>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/Policy.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"
#include "Cloud/Task.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace artificialbeecolony
{

struct Parameters
{
    std::uint32_t populationSize;
    std::uint32_t maxIterationsWithoutChange;
    std::uint32_t maxIterations;
};

std::string toString(const Parameters &parameters);

class ArtificialBeeColonyBase : public PolicyBase
{
  public:
    ArtificialBeeColonyBase(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                            mapping::MappingAssessorPtr &&mappingAssessor, const logger::LoggerPtr &logger);

  protected:
    NodeToTaskMapping createNewSolution(const TaskPtrVec &tasks);
    std::vector<NodeId> findNotEmptyNodeIds(const NodeToTaskMapping &solution);
    std::vector<NodeId> findFeasibleNodeIds(const NodeToTaskMapping &solution, const TaskPtr &task);

  private:
    struct BeeSolution
    {
        NodeToTaskMapping solution;
        std::uint32_t counter;
    };

    struct BestSolution
    {
        NodeToTaskMapping solution;
        double value;
    };

    BestSolution generateRandomSolutions(const TaskPtrVec &tasks);
    virtual NodeToTaskMapping createRandomSolution(const TaskPtrVec &tasks);
    virtual NodeToTaskMapping getNewSolutionFromNeighbourhood(const NodeToTaskMapping &solution);
    NodeToTaskMapping adjustSolutionWithExistingTasks(const NodeToTaskMapping &solution);
    std::vector<NodeId> extractFreeNodeIds();

    const Parameters parameters;
    const mapping::MappingAssessorPtr mappingAssessor;

    std::vector<BeeSolution> solutions;
};

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
