#include "ShortestRemainingTimeFirstBuilder.hpp"

#include <sstream>

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
    const configuration::PolicyConfiguration policyConfiguration, const bool withMigrationsFixing)
    : PolicyBuilderBase(policyConfiguration), withMigrationsFixing(withMigrationsFixing)
{
}

PolicyBuilderPtr ShortestRemainingTimeFirstBuilder::clone()
{
    return std::make_shared<ShortestRemainingTimeFirstBuilder>(policyConfiguration, withMigrationsFixing);
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
        return std::make_unique<ShortestRemainingTimeFirstWithMigrationsAndPreemptions>(infrastructure, logger,
                                                                                        withMigrationsFixing);
    }

    return nullptr;
}

std::string ShortestRemainingTimeFirstBuilder::toString() const
{
    std::stringstream ss;
    ss << "ShortestRemainingTimeFirst" << configuration::toString(policyConfiguration) << "_" << std::boolalpha
       << withMigrationsFixing;

    return ss.str();
}

} // namespace shortestremainingtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
