#include "OfflineArtificialBeeColony.hpp"

#include <utility>

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace artificialbeecolony
{

OfflineArtificialBeeColony::OfflineArtificialBeeColony(const InfrastructureCPtr &infrastructure,
                                                       const Parameters &parameters,
                                                       mapping::MappingAssessorPtr &&mappingAssessor,
                                                       const logger::LoggerPtr &logger)
    : PolicyBase(infrastructure, logger), parameters(parameters), mappingAssessor(std::move(mappingAssessor))
{
}

NodeToTaskMapping OfflineArtificialBeeColony::buildNodeToTaskMapping(const TaskPtrVec &tasks)
{
    return {};
}

std::string OfflineArtificialBeeColony::toString() const
{
    return "";
}

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
