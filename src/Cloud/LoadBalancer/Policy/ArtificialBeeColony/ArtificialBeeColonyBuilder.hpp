#pragma once

#include "ArtificialBeeColony.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBuilderBase.hpp"
#include "Configuration/ConfigurationReader.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace simulatedannealing
{

class ArtificialBeeColonyBuilder : public PolicyBuilderBase
{
  public:
    ArtificialBeeColonyBuilder(const configuration::PolicyConfiguration &policyConfiguration,
                               const configuration::Assessment assessment, const Parameters &parameters);

    PolicyBuilderPtr clone() override;

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
