#pragma once

#include <optional>

#include "Cloud/LoadBalancer/Policy/SimulatedAnnealing/SimulatedAnnealing.hpp"
#include "ConfigurationReader.hpp"
#include "JsonAlgorithmConfigurator.hpp"

namespace configuration
{

class SimulatedAnnealingConfigurator : public JsonAlgorithmConfigurator
{
  public:
    cloud::loadbalancer::policy::PolicyBuilderPtr configure(const nlohmann::json &configuration) override;

  private:
    std::optional<cloud::loadbalancer::policy::simulatedannealing::Parameters> readParameters(
        const nlohmann::json &configuration);
    std::optional<Assessment> readAssessment(const nlohmann::json &configuration);
    std::optional<PolicyConfiguration> readPolicyConfiguration(const nlohmann::json &configuration);
};

} // namespace configuration
