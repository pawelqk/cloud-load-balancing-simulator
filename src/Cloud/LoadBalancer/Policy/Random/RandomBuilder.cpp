#include "RandomBuilder.hpp"

#include "Random.hpp"
#include "RandomWithMigrationsAndPreemptions.hpp"

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

PolicyBuilderPtr RandomBuilder::clone()
{
    return std::make_shared<RandomBuilder>(policyConfiguration);
}

PolicyPtr RandomBuilder::build(const logger::LoggerPtr &logger)
{
    using configuration::PolicyConfiguration;
    switch (policyConfiguration)
    {
    case PolicyConfiguration::Offline:
    case PolicyConfiguration::Online:
        return std::make_unique<Random>(infrastructure, logger);
    case PolicyConfiguration::OnlineWithMigrationsAndPreemptions:
        return std::make_unique<RandomWithMigrationsAndPreemptions>(infrastructure, logger);
    }

    return nullptr;
}

std::string RandomBuilder::toString() const
{
    return "Random" + configuration::toString(policyConfiguration);
}

} // namespace random
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
