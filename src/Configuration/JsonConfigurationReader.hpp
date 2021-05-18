#pragma once

#include <stdexcept>
#include <vector>

#include <nlohmann/json.hpp>

#include "Cloud/LoadBalancer/Policy/PolicyBuilder.hpp"
#include "ConfigurationReader.hpp"
#include "JsonAlgorithmConfigurator.hpp"

namespace configuration
{

class JsonConfigurationReader : public ConfigurationReader
{
  public:
    JsonConfigurationReader(const nlohmann::json &json);

    GeneralConfiguration readGeneralConfiguration() override;
    std::vector<cloud::loadbalancer::policy::PolicyBuilderPtr> readPolicies() override;

  private:
    JsonAlgorithmConfiguratorPtr getAlgorithmConfigurator(const std::string &algorithmName);

    const nlohmann::json json;
    double penaltyFactor;
};

} // namespace configuration
