#include "SimulatedAnnealingBuilder.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace builders
{

SimulatedAnnealingBuilder::SimulatedAnnealingBuilder(const instance::PolicyConfiguration policyConfiguration,
                                                     SimulatedAnnealing::Parameters &&parameters)
    : PolicyBuilderBase(policyConfiguration), parameters(std::move(parameters))
{
}

PolicyPtr SimulatedAnnealingBuilder::build(const logger::LoggerPtr &logger)
{
    return std::make_unique<SimulatedAnnealing>(infrastructure, std::move(parameters), logger);
}

std::string SimulatedAnnealingBuilder::toString() const
{
    return "SimulatedAnnealing";
}

} // namespace builders
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
