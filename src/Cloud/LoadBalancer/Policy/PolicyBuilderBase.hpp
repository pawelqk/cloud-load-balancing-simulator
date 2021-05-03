#pragma once

#include <list>
#include <map>
#include <optional>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/TimingService.hpp"
#include "Configuration/ConfigurationReader.hpp"
#include "Configuration/Instance.hpp"
#include "PolicyBuilder.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

class PolicyBuilderBase : public PolicyBuilder
{
  public:
    PolicyBuilderBase(const configuration::PolicyConfiguration policyConfiguration);

    void setInstance(const configuration::Instance &instance) override;
    void setInfrastructure(const InfrastructureCPtr &infrastructure) override;
    void setDifferenceCalculator(const mapping::DifferenceCalculatorPtr &differenceCalculator) override;
    void setTimingService(const TimingServicePtr &timingService) override;

  protected:
    const configuration::PolicyConfiguration policyConfiguration;

    std::optional<configuration::Instance> instance;
    InfrastructureCPtr infrastructure;
    mapping::DifferenceCalculatorPtr differenceCalculator;
    TimingServicePtr timingService;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
