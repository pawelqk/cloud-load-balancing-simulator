#include "PolicyBuilderBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace builders
{

PolicyBuilderBase::PolicyBuilderBase(const instance::PolicyConfiguration policyConfiguration)
    : policyConfiguration(policyConfiguration)
{
}

void PolicyBuilderBase::setInstance(const instance::Instance &instance)
{
    if (policyConfiguration == instance::PolicyConfiguration::Offline)
        this->instance.emplace(instance);
}

void PolicyBuilderBase::setInfrastructure(const InfrastructureCPtr &infrastructure)
{
    this->infrastructure = infrastructure;
}

} // namespace builders
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
