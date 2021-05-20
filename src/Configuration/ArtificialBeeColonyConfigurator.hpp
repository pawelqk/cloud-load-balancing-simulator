#pragma once

#include <optional>

#include "Cloud/LoadBalancer/Policy/ArtificialBeeColony/ArtificialBeeColonyBase.hpp"
#include "ConfigurationReader.hpp"
#include "JsonAlgorithmConfigurator.hpp"

namespace configuration
{

class ArtificialBeeColonyConfigurator : public JsonAlgorithmConfigurator
{
  public:
    ArtificialBeeColonyConfigurator(const double penaltyFactor);
    cloud::loadbalancer::policy::PolicyBuilderPtr configure(const nlohmann::json &configuration) override;

  private:
    std::optional<cloud::loadbalancer::policy::artificialbeecolony::Parameters> readParameters(
        const nlohmann::json &configuration);
    std::optional<Assessment> readAssessment(const nlohmann::json &configuration);
    std::optional<PolicyConfiguration> readPolicyConfiguration(const nlohmann::json &configuration);

    const double penaltyFactor;
};

} // namespace configuration
