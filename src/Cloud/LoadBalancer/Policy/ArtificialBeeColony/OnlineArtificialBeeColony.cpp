#include "OnlineArtificialBeeColony.hpp"

#include <utility>

#include "Utility/RandomNumberGenerator.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace artificialbeecolony
{

OnlineArtificialBeeColony::OnlineArtificialBeeColony(const InfrastructureCPtr &infrastructure,
                                                     const Parameters &parameters,
                                                     mapping::MappingAssessorPtr &&mappingAssessor,
                                                     const logger::LoggerPtr &logger)
    : ArtificialBeeColonyBase(infrastructure, parameters, std::move(mappingAssessor), logger)
{
}

std::string OnlineArtificialBeeColony::toString() const
{
    return "OnlineArtificialBeeColony";
}

NodeToTaskMapping OnlineArtificialBeeColony::buildNodeToTaskMappingInternal(const TaskPtrVec &tasks)
{
    return adjustSolutionWithExistingTasks(createNewSolution(tasks));
}

NodeToTaskMapping OnlineArtificialBeeColony::adjustSolutionWithExistingTasks(const NodeToTaskMapping &solution)
{
    auto adjustedSolution = solution;

    const auto &nodes = infrastructure->getNodes();
    for (auto &&node : nodes)
    {
        const auto task = node->getTask();
        if (task != nullptr)
            adjustedSolution[node->getId()].push_front(task);
    }

    return adjustedSolution;
}

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
