#include "OnlineArtificialBeeColonyWithMigrationsAndPreemptions.hpp"

#include <utility>

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
    : PolicyBase(infrastructure, logger), parameters(parameters), mappingAssessor(std::move(mappingAssessor))
{
}

NodeToTaskMapping OnlineArtificialBeeColonyWithMigrationsAndPreemptions::buildNodeToTaskMapping(const TaskPtrVec &tasks)
{
    return {};
}

std::string OnlineArtificialBeeColonyWithMigrationsAndPreemptions::toString() const
{
    return "";
}

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
