#pragma once

#include <memory>
#include <string>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Policy/Policy.hpp"
#include "Instance/Instance.hpp"
#include "Logger/Logger.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace builders
{

class PolicyBuilder
{
  public:
    virtual ~PolicyBuilder() = default;

    virtual void setInstance(const instance::Instance &instance) = 0;
    virtual void setInfrastructure(const InfrastructureCPtr &) = 0;
    virtual PolicyPtr build(const logger::LoggerPtr &) = 0;

    virtual std::string toString() const = 0;
};

using PolicyBuilderPtr = std::shared_ptr<PolicyBuilder>;

} // namespace builders
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
