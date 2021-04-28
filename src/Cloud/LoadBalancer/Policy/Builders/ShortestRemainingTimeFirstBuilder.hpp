#pragma once

#include <list>
#include <map>
#include <memory>
#include <optional>

#include "Cloud/Infrastructure.hpp"
#include "Instance/Instance.hpp"
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

class ShortestRemainingTimeFirstBuilder : public PolicyBuilderBase
{
  public:
    ShortestRemainingTimeFirstBuilder(const instance::PolicyConfiguration policyConfiguration);
    PolicyPtr build(const logger::LoggerPtr &logger) override;

    std::string toString() const override;
};

} // namespace builders
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
