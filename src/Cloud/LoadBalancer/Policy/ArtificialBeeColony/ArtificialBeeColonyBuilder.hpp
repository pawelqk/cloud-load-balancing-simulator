#pragma once

#include "ArtificialBeeColonyBase.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBuilderBase.hpp"
#include "Configuration/ConfigurationReader.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace artificialbeecolony
{

class ArtificialBeeColonyBuilder : public PolicyBuilderBase
{
  public:
    ArtificialBeeColonyBuilder(const configuration::PolicyConfiguration &policyConfiguration,
                               const configuration::Assessment assessment, const Parameters &parameters,
                               const double penaltyFactor);

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

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
