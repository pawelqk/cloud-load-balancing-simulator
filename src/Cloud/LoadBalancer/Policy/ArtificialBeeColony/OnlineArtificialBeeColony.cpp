#include "OnlineArtificialBeeColony.hpp"

#include <utility>

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
    : PolicyBase(infrastructure, logger), parameters(parameters), mappingAssessor(std::move(mappingAssessor))
{
}

NodeToTaskMapping OnlineArtificialBeeColony::buildNodeToTaskMapping(const TaskPtrVec &tasks)
{
    return {};
}

std::string OnlineArtificialBeeColony::toString() const
{
    return "";
}

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
