#include "RoundRobinBuilder.hpp"

#include "RoundRobin.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace roundrobin
{

RoundRobinBuilder::RoundRobinBuilder(const configuration::PolicyConfiguration policyConfiguration)
    : PolicyBuilderBase(policyConfiguration)
{
}

PolicyBuilderPtr RoundRobinBuilder::clone()
{
    return std::make_shared<RoundRobinBuilder>(policyConfiguration);
}

PolicyPtr RoundRobinBuilder::build(const logger::LoggerPtr &logger)
{
    return std::make_unique<RoundRobin>(infrastructure, logger);
}

std::string RoundRobinBuilder::toString() const
{
    return "RoundRobin" + configuration::toString(policyConfiguration);
}

} // namespace roundrobin
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
