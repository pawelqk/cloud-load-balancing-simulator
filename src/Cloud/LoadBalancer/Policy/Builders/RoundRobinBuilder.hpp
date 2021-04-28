#pragma once

#include <string>

#include "Logger/Logger.hpp"
#include "PolicyBuilderBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace builders
{

class RoundRobinBuilder : public PolicyBuilderBase
{
  public:
    RoundRobinBuilder(const instance::PolicyConfiguration policyConfiguration);
    PolicyPtr build(const logger::LoggerPtr &logger) override;

    std::string toString() const override;
};

} // namespace builders
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
