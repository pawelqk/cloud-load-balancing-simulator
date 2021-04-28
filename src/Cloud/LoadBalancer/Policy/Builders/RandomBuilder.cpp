#include "RandomBuilder.hpp"

#include "Cloud/LoadBalancer/Policy/Random.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace builders
{

RandomBuilder::RandomBuilder(const instance::PolicyConfiguration policyConfiguration)
    : PolicyBuilderBase(policyConfiguration)
{
}

PolicyPtr RandomBuilder::build(const logger::LoggerPtr &logger)
{
    return std::make_unique<Random>(infrastructure, logger);
}

std::string RandomBuilder::toString() const
{
    return "Random";
}

} // namespace builders
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
