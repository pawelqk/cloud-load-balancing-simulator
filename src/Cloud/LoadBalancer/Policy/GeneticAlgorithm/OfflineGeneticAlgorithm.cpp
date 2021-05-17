#include "OfflineGeneticAlgorithm.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

OfflineGeneticAlgorithm::OfflineGeneticAlgorithm(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                                                 const std::shared_ptr<mapping::MappingAssessor> &mappingAssessor,
                                                 const configuration::Instance &instance,
                                                 const logger::LoggerPtr &logger)
    : PolicyBase(infrastructure, logger), parameters(parameters), mappingAssessor(mappingAssessor), instance(instance)
{
}

NodeToTaskMapping OfflineGeneticAlgorithm::buildNodeToTaskMappingInternal(const TaskPtrVec &tasks)
{
    return {};
}

std::string OfflineGeneticAlgorithm::toString() const
{
    return "";
}

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
