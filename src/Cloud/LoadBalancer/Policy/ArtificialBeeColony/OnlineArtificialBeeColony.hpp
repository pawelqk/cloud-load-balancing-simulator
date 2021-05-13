#pragma once

#include <cstdint>

#include "ArtificialBeeColonyBase.hpp"
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
namespace artificialbeecolony
{

class OnlineArtificialBeeColony : public PolicyBase
{
  public:
    OnlineArtificialBeeColony(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                              mapping::MappingAssessorPtr &&mappingAssessor, const logger::LoggerPtr &logger);

    NodeToTaskMapping buildNodeToTaskMapping(const TaskPtrVec &tasks) override;

    std::string toString() const override;

  protected:
    BestSolution generateRandomSolutions(const TaskPtrVec &tasks);
    NodeToTaskMapping createRandomSolution(const TaskPtrVec &tasks);
    NodeToTaskMapping getNewSolutionFromNeighbourhood(const NodeToTaskMapping &solution);
    NodeToTaskMapping adjustSolutionWithExistingTasks(const NodeToTaskMapping &solution);
    std::vector<NodeId> extractFreeNodeIds();
    std::vector<NodeId> findNotEmptyNodeIds(const NodeToTaskMapping &solution);
    std::vector<NodeId> findFeasibleNodeIds(const NodeToTaskMapping &solution, const TaskPtr &task);

    const Parameters parameters;
    const mapping::MappingAssessorPtr mappingAssessor;

    std::vector<BeeSolution> solutions;
};

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
