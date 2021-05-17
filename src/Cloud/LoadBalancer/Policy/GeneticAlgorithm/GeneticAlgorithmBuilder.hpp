#pragma once

#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBuilderBase.hpp"
#include "Configuration/ConfigurationReader.hpp"
#include "GeneticAlgorithmBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

class GeneticAlgorithmBuilder : public PolicyBuilderBase
{
  public:
    GeneticAlgorithmBuilder(const configuration::PolicyConfiguration &policyConfiguration,
                            const configuration::Assessment assessment, const Parameters &parameters);

    PolicyBuilderPtr clone() override;

    PolicyPtr build(const logger::LoggerPtr &logger) override;

    std::string toString() const override;

  private:
    std::shared_ptr<mapping::MappingAssessor> buildAssessor();

    const configuration::Assessment assessment;
    Parameters parameters;
};

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
