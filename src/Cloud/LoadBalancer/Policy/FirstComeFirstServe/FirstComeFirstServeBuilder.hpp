#pragma once

#include <string>

#include "Cloud/LoadBalancer/Policy/PolicyBuilderBase.hpp"
#include "Configuration/ConfigurationReader.hpp"
#include "Logger/Logger.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace firstcomefirstserve
{

class FirstComeFirstServeBuilder : public PolicyBuilderBase
{
  public:
    FirstComeFirstServeBuilder(const configuration::PolicyConfiguration policyConfiguration);

    PolicyBuilderPtr clone() override;

    PolicyPtr build(const logger::LoggerPtr &logger, const std::uint_fast64_t seed,
                    const double penaltyFactor) override;

    std::string toString() const override;
};

} // namespace firstcomefirstserve
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
