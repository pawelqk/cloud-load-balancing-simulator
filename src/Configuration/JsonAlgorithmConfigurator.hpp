#pragma once

#include <memory>

#include <nlohmann/json.hpp>

#include "Cloud/LoadBalancer/Policy/PolicyBuilder.hpp"

namespace configuration
{

class JsonAlgorithmConfigurator
{
  public:
    virtual cloud::loadbalancer::policy::PolicyBuilderPtr configure(const nlohmann::json &) = 0;
};

using JsonAlgorithmConfiguratorPtr = std::unique_ptr<JsonAlgorithmConfigurator>;

} // namespace configuration
