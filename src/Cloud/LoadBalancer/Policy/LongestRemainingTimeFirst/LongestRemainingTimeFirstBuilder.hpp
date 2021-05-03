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
namespace longestremainingtimefirst
{

class LongestRemainingTimeFirstBuilder : public PolicyBuilderBase
{
  public:
    LongestRemainingTimeFirstBuilder(const configuration::PolicyConfiguration policyConfiguration);

    PolicyBuilderPtr clone() override;

    PolicyPtr build(const logger::LoggerPtr &logger) override;

    std::string toString() const override;
};

} // namespace longestremainingtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
