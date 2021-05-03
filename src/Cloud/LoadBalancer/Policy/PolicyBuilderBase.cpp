#include "PolicyBuilderBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

PolicyBuilderBase::PolicyBuilderBase(const configuration::PolicyConfiguration policyConfiguration)
    : policyConfiguration(policyConfiguration)
{
}

void PolicyBuilderBase::setInstance(const configuration::Instance &instance)
{
    if (policyConfiguration == configuration::PolicyConfiguration::Offline)
        this->instance.emplace(instance);
}

void PolicyBuilderBase::setInfrastructure(const InfrastructureCPtr &infrastructure)
{
    this->infrastructure = infrastructure;
}

void PolicyBuilderBase::setDifferenceCalculator(const mapping::DifferenceCalculatorPtr &differenceCalculator)
{
    this->differenceCalculator = differenceCalculator;
}

void PolicyBuilderBase::setTimingService(const TimingServicePtr &timingService)
{
    this->timingService = timingService;
}

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
