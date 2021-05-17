#include "OnlineArtificialBeeColonyWithMigrationsAndPreemptions.hpp"

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

OnlineArtificialBeeColonyWithMigrationsAndPreemptions::OnlineArtificialBeeColonyWithMigrationsAndPreemptions(
    const InfrastructureCPtr &infrastructure, const Parameters &parameters,
    mapping::MappingAssessorPtr &&mappingAssessor, const logger::LoggerPtr &logger)
    : ArtificialBeeColonyBase(infrastructure, parameters, std::move(mappingAssessor), logger)
{
}

std::string OnlineArtificialBeeColonyWithMigrationsAndPreemptions::toString() const
{
    return "OnlineArtificialBeeColonyWithMigrationsAndPreemptions";
}

NodeToTaskMapping OnlineArtificialBeeColonyWithMigrationsAndPreemptions::buildNodeToTaskMappingInternal(
    const TaskPtrVec &tasks)
{
    auto allTasks = tasks;
    for (auto &&node : infrastructure->getNodes())
    {
        const auto task = node->getTask();
        if (task != nullptr)
            allTasks.push_back(task);
    }

    return createNewSolution(allTasks);
}

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
