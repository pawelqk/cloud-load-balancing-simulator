#pragma once

#include "Cloud/LoadBalancer/Policy/PolicyBuilderBase.hpp"
#include "Configuration/ConfigurationReader.hpp"
#include "SimulatedAnnealingBase.hpp"

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

    PolicyBuilderPtr clone() override;

    PolicyPtr build(const logger::LoggerPtr &logger) override;

    std::string toString() const override;

  private:
    mapping::MappingAssessorPtr buildAssessor();
    mapping::MappingAssessorPtr buildOfflineAssessor();

    const configuration::Assessment assessment;
    Parameters parameters;
    const double penaltyFactor;
};

} // namespace simulatedannealing
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
