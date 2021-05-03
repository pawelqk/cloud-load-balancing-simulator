#pragma once

#include <string>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBuilderBase.hpp"
#include "Logger/Logger.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace random
{

class RandomBuilder : public PolicyBuilderBase
{
  public:
    RandomBuilder(const configuration::PolicyConfiguration policyConfiguration);
    PolicyPtr build(const logger::LoggerPtr &logger) override;

    std::string toString() const override;
};

} // namespace random
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
