#pragma once

#include <optional>

#include "Cloud/LoadBalancer/Policy/GeneticAlgorithm/GeneticAlgorithmBase.hpp"
#include "ConfigurationReader.hpp"
#include "JsonAlgorithmConfigurator.hpp"

namespace configuration
{

class GeneticAlgorithmConfigurator : public JsonAlgorithmConfigurator
{
  public:
    cloud::loadbalancer::policy::PolicyBuilderPtr configure(const nlohmann::json &configuration) override;

  private:
    std::optional<cloud::loadbalancer::policy::geneticalgorithm::Parameters> readParameters(
        const nlohmann::json &configuration);
    std::optional<Assessment> readAssessment(const nlohmann::json &configuration);
    std::optional<PolicyConfiguration> readPolicyConfiguration(const nlohmann::json &configuration);
};

} // namespace configuration
