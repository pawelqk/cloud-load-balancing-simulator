#include "OnlineGeneticAlgorithmWithMigrationsAndPreemptions.hpp"

#include <utility>

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

OnlineGeneticAlgorithmWithMigrationsAndPreemptions::OnlineGeneticAlgorithmWithMigrationsAndPreemptions(
    const InfrastructureCPtr &infrastructure, const Parameters &parameters,
    mapping::MappingAssessorPtr &&mappingAssessor, const logger::LoggerPtr &logger)
    : PolicyBase(infrastructure, logger), parameters(parameters), mappingAssessor(std::move(mappingAssessor))
{
}

NodeToTaskMapping OnlineGeneticAlgorithmWithMigrationsAndPreemptions::buildNodeToTaskMapping(const TaskPtrVec &tasks)
{
    return {};
}

std::string OnlineGeneticAlgorithmWithMigrationsAndPreemptions::toString() const
{
    return "";
}

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
