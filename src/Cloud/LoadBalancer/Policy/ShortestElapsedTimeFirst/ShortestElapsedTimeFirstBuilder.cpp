#include "ShortestElapsedTimeFirstBuilder.hpp"

#include <sstream>

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
    const configuration::PolicyConfiguration policyConfiguration, const bool withMigrationsFixing)
    : PolicyBuilderBase(policyConfiguration), withMigrationsFixing(withMigrationsFixing)
{
}

PolicyBuilderPtr ShortestElapsedTimeFirstBuilder::clone()
{
    return std::make_shared<ShortestElapsedTimeFirstBuilder>(policyConfiguration, withMigrationsFixing);
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
        return std::make_unique<ShortestElapsedTimeFirstWithMigrationsAndPreemptions>(infrastructure, logger,
                                                                                      withMigrationsFixing);
    }

    return nullptr;
}

std::string ShortestElapsedTimeFirstBuilder::toString() const
{
    std::stringstream ss;
    ss << "ShortestElapsedTimeFirst" << configuration::toString(policyConfiguration) << "_" << std::boolalpha
       << withMigrationsFixing;

    return ss.str();
}

} // namespace shortestelapsedtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
