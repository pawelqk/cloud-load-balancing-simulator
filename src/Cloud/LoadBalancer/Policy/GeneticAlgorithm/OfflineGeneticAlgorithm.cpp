#include "OfflineGeneticAlgorithm.hpp"

#include <utility>

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace artificialbeecolony
{

OfflineGeneticAlgorithm::OfflineGeneticAlgorithm(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                                                 mapping::MappingAssessorPtr &&mappingAssessor,
                                                 const logger::LoggerPtr &logger)
    : PolicyBase(infrastructure, logger), parameters(parameters), mappingAssessor(std::move(mappingAssessor))
{
}

NodeToTaskMapping OfflineGeneticAlgorithm::buildNodeToTaskMapping(const TaskPtrVec &tasks)
{
    return {};
}

std::string OfflineGeneticAlgorithm::toString() const
{
    return "";
}

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
