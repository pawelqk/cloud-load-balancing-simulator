#include "ShortestRemainingTimeFirstBuilder.hpp"

#include "ShortestRemainingTimeFirst.hpp"

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

PolicyPtr ShortestRemainingTimeFirstBuilder::build(const logger::LoggerPtr &logger)
{
    return std::make_unique<ShortestRemainingTimeFirst>(infrastructure, logger);
}

std::string ShortestRemainingTimeFirstBuilder::toString() const
{
    return "ShortestRemainingTimeFirst" + configuration::toString(policyConfiguration);
}

} // namespace shortestremainingtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
