#include "RoundRobinBuilder.hpp"

#include "Cloud/LoadBalancer/Policy/RoundRobin.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace builders
{

RoundRobinBuilder::RoundRobinBuilder(const instance::PolicyConfiguration policyConfiguration)
    : PolicyBuilderBase(policyConfiguration)
{
}

PolicyPtr RoundRobinBuilder::build(const logger::LoggerPtr &logger)
{
    return std::make_unique<RoundRobin>(infrastructure, logger);
}

std::string RoundRobinBuilder::toString() const
{
    return "RoundRobin";
}

} // namespace builders
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
