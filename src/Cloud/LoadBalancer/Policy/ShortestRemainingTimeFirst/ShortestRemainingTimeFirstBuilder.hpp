#pragma once

#include <list>
#include <map>
#include <memory>
#include <optional>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBuilderBase.hpp"
#include "Configuration/Instance.hpp"
#include "Logger/Logger.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace shortestremainingtimefirst
{

class ShortestRemainingTimeFirstBuilder : public PolicyBuilderBase
{
  public:
    ShortestRemainingTimeFirstBuilder(const configuration::PolicyConfiguration policyConfiguration);
    PolicyPtr build(const logger::LoggerPtr &logger) override;

    std::string toString() const override;
};

} // namespace shortestremainingtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
