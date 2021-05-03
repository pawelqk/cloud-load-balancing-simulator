#include "RandomBuilder.hpp"

#include "Random.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace random
{

RandomBuilder::RandomBuilder(const configuration::PolicyConfiguration policyConfiguration)
    : PolicyBuilderBase(policyConfiguration)
{
}

PolicyPtr RandomBuilder::build(const logger::LoggerPtr &logger)
{
    return std::make_unique<Random>(infrastructure, logger);
}

std::string RandomBuilder::toString() const
{
    return "Random" + configuration::toString(policyConfiguration);
}

} // namespace random
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
