#pragma once

#include <list>
#include <map>
#include <optional>

#include "Cloud/Infrastructure.hpp"
#include "Instance/Instance.hpp"
#include "PolicyBuilder.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace builders
{

class PolicyBuilderBase : public PolicyBuilder
{
  public:
    PolicyBuilderBase(const instance::PolicyConfiguration policyConfiguration);

    void setInstance(const instance::Instance &instance) override;
    void setInfrastructure(const InfrastructureCPtr &infrastructure) override;

  protected:
    const instance::PolicyConfiguration policyConfiguration;

    std::optional<instance::Instance> instance;
    InfrastructureCPtr infrastructure;
};

} // namespace builders
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
