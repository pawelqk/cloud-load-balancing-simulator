#pragma once

#include <list>
#include <map>
#include <memory>
#include <optional>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Policy/SimulatedAnnealing.hpp"
#include "Instance/Instance.hpp"
#include "PolicyBuilderBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace builders
{

class SimulatedAnnealingBuilder : public PolicyBuilderBase
{
  public:
    SimulatedAnnealingBuilder(const instance::PolicyConfiguration policyConfiguration,
                              SimulatedAnnealing::Parameters &&parameters);

    PolicyPtr build(const logger::LoggerPtr &logger) override;

    std::string toString() const override;

  private:
    SimulatedAnnealing::Parameters parameters;
};

} // namespace builders
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
