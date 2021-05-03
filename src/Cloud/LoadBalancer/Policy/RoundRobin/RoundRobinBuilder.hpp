#pragma once

#include <string>

#include "Cloud/LoadBalancer/Policy/PolicyBuilderBase.hpp"
#include "Logger/Logger.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace roundrobin
{

class RoundRobinBuilder : public PolicyBuilderBase
{
  public:
    RoundRobinBuilder(const configuration::PolicyConfiguration policyConfiguration);
    PolicyPtr build(const logger::LoggerPtr &logger) override;

    std::string toString() const override;
};

} // namespace roundrobin
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
