#pragma once

#include <memory>
#include <string>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/DifferenceCalculator.hpp"
#include "Cloud/LoadBalancer/Policy/Policy.hpp"
#include "Cloud/TimingService.hpp"
#include "Configuration/Instance.hpp"
#include "Logger/Logger.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

class PolicyBuilder
{
  public:
    virtual ~PolicyBuilder() = default;

    virtual void setInstance(const configuration::Instance &instance) = 0;
    virtual void setInfrastructure(const InfrastructureCPtr &) = 0;
    virtual void setDifferenceCalculator(const mapping::DifferenceCalculatorPtr &) = 0;
    virtual void setTimingService(const TimingServicePtr &) = 0;
    virtual PolicyPtr build(const logger::LoggerPtr &) = 0;

    virtual std::string toString() const = 0;
};

using PolicyBuilderPtr = std::shared_ptr<PolicyBuilder>;

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
