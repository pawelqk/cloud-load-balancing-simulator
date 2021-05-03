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
namespace shortestelapsedtimefirst
{

class ShortestElapsedTimeFirstBuilder : public PolicyBuilderBase
{
  public:
    ShortestElapsedTimeFirstBuilder(const configuration::PolicyConfiguration policyConfiguration);

    PolicyBuilderPtr clone() override;

    PolicyPtr build(const logger::LoggerPtr &logger) override;

    std::string toString() const override;
};

} // namespace shortestelapsedtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
