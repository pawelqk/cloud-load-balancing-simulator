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
    LongestRemainingTimeFirstBuilder(const configuration::PolicyConfiguration policyConfiguration,
                                     const bool withMigrationsFixing);

    PolicyBuilderPtr clone() override;

    PolicyPtr build(const logger::LoggerPtr &logger) override;

    std::string toString() const override;

  private:
    const bool withMigrationsFixing;
};

} // namespace longestremainingtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
