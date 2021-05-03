#include "ShortestElapsedTimeFirstBuilder.hpp"

#include "ShortestElapsedTimeFirst.hpp"
#include "ShortestElapsedTimeFirstWithMigrationsAndPreemptions.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace shortestelapsedtimefirst
{

ShortestElapsedTimeFirstBuilder::ShortestElapsedTimeFirstBuilder(
    const configuration::PolicyConfiguration policyConfiguration)
    : PolicyBuilderBase(policyConfiguration)
{
}

PolicyBuilderPtr ShortestElapsedTimeFirstBuilder::clone()
{
    return std::make_shared<ShortestElapsedTimeFirstBuilder>(policyConfiguration);
}

PolicyPtr ShortestElapsedTimeFirstBuilder::build(const logger::LoggerPtr &logger)
{
    using configuration::PolicyConfiguration;
    switch (policyConfiguration)
    {
    case PolicyConfiguration::Offline:
    case PolicyConfiguration::Online:
        return std::make_unique<ShortestElapsedTimeFirst>(infrastructure, logger);
    case PolicyConfiguration::OnlineWithMigrationsAndPreemptions:
        return std::make_unique<ShortestElapsedTimeFirstWithMigrationsAndPreemptions>(infrastructure, logger, true);
    }

    return nullptr;
}

std::string ShortestElapsedTimeFirstBuilder::toString() const
{
    return "ShortestElapsedTimeFirst" + configuration::toString(policyConfiguration);
}

} // namespace shortestelapsedtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
