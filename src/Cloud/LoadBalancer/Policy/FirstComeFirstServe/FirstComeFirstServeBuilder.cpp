#include "FirstComeFirstServeBuilder.hpp"

#include "FirstComeFirstServe.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace firstcomefirstserve
{

FirstComeFirstServeBuilder::FirstComeFirstServeBuilder(const configuration::PolicyConfiguration policyConfiguration)
    : PolicyBuilderBase(policyConfiguration)
{
}

PolicyBuilderPtr FirstComeFirstServeBuilder::clone()
{
    return std::make_shared<FirstComeFirstServeBuilder>(policyConfiguration);
}

PolicyPtr FirstComeFirstServeBuilder::build(const logger::LoggerPtr &logger)
{
    return std::make_unique<FirstComeFirstServe>(infrastructure, logger);
}

std::string FirstComeFirstServeBuilder::toString() const
{
    return "FirstComeFirstServe" + configuration::toString(policyConfiguration);
}

} // namespace firstcomefirstserve
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
