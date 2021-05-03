#pragma once

#include "ConfigurationReader.hpp"
#include "JsonAlgorithmConfigurator.hpp"

namespace configuration
{

class GenericConfigurator : public JsonAlgorithmConfigurator
{
  public:
    GenericConfigurator(const Policy policy);

    cloud::loadbalancer::policy::PolicyBuilderPtr configure(const nlohmann::json &configuration) override;

  private:
    cloud::loadbalancer::policy::PolicyBuilderPtr build(const PolicyConfiguration policyConfiguration);

    const Policy policy;
};

} // namespace configuration
