#include "LongestRemainingTimeFirstBuilder.hpp"

#include <sstream>

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
    const configuration::PolicyConfiguration policyConfiguration, const bool withMigrationsFixing)
    : PolicyBuilderBase(policyConfiguration), withMigrationsFixing(withMigrationsFixing)
{
}

PolicyBuilderPtr LongestRemainingTimeFirstBuilder::clone()
{
    return std::make_shared<LongestRemainingTimeFirstBuilder>(policyConfiguration, withMigrationsFixing);
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
        return std::make_unique<LongestRemainingTimeFirstWithMigrationsAndPreemptions>(infrastructure, logger,
                                                                                       withMigrationsFixing);
    }

    return nullptr;
}

std::string LongestRemainingTimeFirstBuilder::toString() const
{
    std::stringstream ss;
    ss << "LongestRemainingTimeFirst" << configuration::toString(policyConfiguration) << "_" << std::boolalpha
       << withMigrationsFixing;

    return ss.str();
}

} // namespace longestremainingtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
