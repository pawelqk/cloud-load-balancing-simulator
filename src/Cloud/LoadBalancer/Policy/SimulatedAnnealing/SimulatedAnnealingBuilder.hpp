#pragma once

#include "Cloud/LoadBalancer/Policy/PolicyBuilderBase.hpp"
#include "Configuration/ConfigurationReader.hpp"
#include "SimulatedAnnealing.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace simulatedannealing
{

class SimulatedAnnealingBuilder : public PolicyBuilderBase
{
  public:
    SimulatedAnnealingBuilder(const configuration::PolicyConfiguration &policyConfiguration,
                              const configuration::Assessment assessment, const Parameters &parameters);

    PolicyPtr build(const logger::LoggerPtr &logger) override;

    std::string toString() const override;

  private:
    mapping::MappingAssessorPtr buildAssessor();

    const configuration::Assessment assessment;
    Parameters parameters;
};

} // namespace simulatedannealing
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
