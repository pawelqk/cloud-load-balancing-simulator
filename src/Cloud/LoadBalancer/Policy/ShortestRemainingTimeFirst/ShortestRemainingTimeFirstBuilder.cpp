#include "ShortestRemainingTimeFirstBuilder.hpp"

#include "ShortestRemainingTimeFirst.hpp"
#include "ShortestRemainingTimeFirstWithMigrationsAndPreemptions.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace shortestremainingtimefirst
{

ShortestRemainingTimeFirstBuilder::ShortestRemainingTimeFirstBuilder(
    const configuration::PolicyConfiguration policyConfiguration)
    : PolicyBuilderBase(policyConfiguration)
{
}

PolicyBuilderPtr ShortestRemainingTimeFirstBuilder::clone()
{
    return std::make_shared<ShortestRemainingTimeFirstBuilder>(policyConfiguration);
}

PolicyPtr ShortestRemainingTimeFirstBuilder::build(const logger::LoggerPtr &logger)
{
    using configuration::PolicyConfiguration;
    switch (policyConfiguration)
    {
    case PolicyConfiguration::Offline:
    case PolicyConfiguration::Online:
        return std::make_unique<ShortestRemainingTimeFirst>(infrastructure, logger);
    case PolicyConfiguration::OnlineWithMigrationsAndPreemptions:
        return std::make_unique<ShortestRemainingTimeFirstWithMigrationsAndPreemptions>(infrastructure, logger, true);
    }

    return nullptr;
}

std::string ShortestRemainingTimeFirstBuilder::toString() const
{
    return "ShortestRemainingTimeFirst" + configuration::toString(policyConfiguration);
}

} // namespace shortestremainingtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
