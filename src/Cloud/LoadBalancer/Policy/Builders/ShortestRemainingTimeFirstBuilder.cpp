#include "ShortestRemainingTimeFirstBuilder.hpp"

#include "Cloud/LoadBalancer/Policy/ShortestRemainingTimeFirst.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace builders
{

ShortestRemainingTimeFirstBuilder::ShortestRemainingTimeFirstBuilder(
    const instance::PolicyConfiguration policyConfiguration)
    : PolicyBuilderBase(policyConfiguration)
{
}

PolicyPtr ShortestRemainingTimeFirstBuilder::build(const logger::LoggerPtr &logger)
{
    return std::make_unique<ShortestRemainingTimeFirst>(infrastructure, logger);
}

std::string ShortestRemainingTimeFirstBuilder::toString() const
{
    return "ShortestRemainingTimeFirst";
}

} // namespace builders
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
