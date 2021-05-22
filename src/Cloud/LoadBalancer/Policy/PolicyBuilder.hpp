#pragma once

#include <memory>
#include <string>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/DifferenceCalculator.hpp"
#include "Cloud/LoadBalancer/Policy/Policy.hpp"
#include "Cloud/TimingService.hpp"
#include "Configuration/Instance.hpp"
#include "Logger/Logger.hpp"
#include "Utility/RandomNumberGenerator.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

class PolicyBuilder;

using PolicyBuilderPtr = std::shared_ptr<PolicyBuilder>;

class PolicyBuilder
{
  public:
    virtual ~PolicyBuilder() = default;

    virtual PolicyBuilderPtr clone() = 0;

    virtual void setInstance(const configuration::Instance &instance) = 0;
    virtual void setRandomNumberGenerator(const utility::RandomNumberGeneratorPtr &) = 0;
    virtual void setPenaltyFactor(const double) = 0;
    virtual void setInfrastructure(const InfrastructurePtr &) = 0;
    virtual void setDifferenceCalculator(const mapping::DifferenceCalculatorPtr &) = 0;
    virtual void setTimingService(const TimingServicePtr &) = 0;
    virtual PolicyPtr build(const logger::LoggerPtr &) = 0;

    virtual std::string toString() const = 0;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
