#include "OnlineGeneticAlgorithm.hpp"

#include <cmath>
#include <random>

#include "Utility/RandomNumberGenerator.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

OnlineGeneticAlgorithm::OnlineGeneticAlgorithm(const InfrastructureCPtr &infrastructure, Parameters &&parameters,
                                               const logger::LoggerPtr &logger)
    : PolicyBase(infrastructure, logger), parameters(std::move(parameters)), logger(logger)
{
}

NodeToTaskMapping OnlineGeneticAlgorithm::buildNodeToTaskMapping(const TaskPtrVec &tasks)
{
    logger->debug("Mapping %u tasks", tasks.size());
    NodeToTaskMapping mappingActions;

    return mappingActions;
}

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
