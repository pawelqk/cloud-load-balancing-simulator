#include "GeneticAlgorithm.hpp"

#include <cmath>
#include <random>

#include "Utility/RandomNumberGenerator.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

GeneticAlgorithm::GeneticAlgorithm(const InfrastructureCPtr &infrastructure, Parameters &&parameters,
                                   const logger::LoggerPtr &logger)
    : PolicyBase(infrastructure), parameters(std::move(parameters)), logger(logger)
{
}

NodeToTaskMapping GeneticAlgorithm::buildNodeToTaskMapping(const TaskPtrVec &tasks)
{
    logger->debug("Mapping %u tasks", tasks.size());
    NodeToTaskMapping mappingActions;

    return mappingActions;
}

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
