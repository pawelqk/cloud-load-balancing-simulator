#include "LongestRemainingTimeFirstBuilder.hpp"

#include "LongestRemainingTimeFirst.hpp"
#include "LongestRemainingTimeFirstWithMigrationsAndPreemptions.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace longestremainingtimefirst
{

LongestRemainingTimeFirstBuilder::LongestRemainingTimeFirstBuilder(
    const configuration::PolicyConfiguration policyConfiguration)
    : PolicyBuilderBase(policyConfiguration)
{
}

PolicyBuilderPtr LongestRemainingTimeFirstBuilder::clone()
{
    return std::make_shared<LongestRemainingTimeFirstBuilder>(policyConfiguration);
}

PolicyPtr LongestRemainingTimeFirstBuilder::build(const logger::LoggerPtr &logger)
{
    using configuration::PolicyConfiguration;
    switch (policyConfiguration)
    {
    case PolicyConfiguration::Offline:
    case PolicyConfiguration::Online:
        return std::make_unique<LongestRemainingTimeFirst>(infrastructure, logger);
    case PolicyConfiguration::OnlineWithMigrationsAndPreemptions:
        return std::make_unique<LongestRemainingTimeFirstWithMigrationsAndPreemptions>(infrastructure, logger, true);
    }

    return nullptr;
}

std::string LongestRemainingTimeFirstBuilder::toString() const
{
    return "LongestRemainingTimeFirst" + configuration::toString(policyConfiguration);
}

} // namespace longestremainingtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
