#include "GeneticAlgorithmConfigurator.hpp"

#include "Cloud/LoadBalancer/Policy/GeneticAlgorithm/GeneticAlgorithmBuilder.hpp"

namespace configuration
{

cloud::loadbalancer::policy::PolicyBuilderPtr GeneticAlgorithmConfigurator::configure(
    const nlohmann::json &configuration)
{
    const auto parameters = readParameters(configuration);
    if (parameters == std::nullopt)
        return nullptr;

    const auto assessment = readAssessment(configuration);
    if (assessment == std::nullopt)
        return nullptr;

    const auto policyConfiguration = readPolicyConfiguration(configuration);
    if (policyConfiguration == std::nullopt)
        return nullptr;

    return std::make_shared<cloud::loadbalancer::policy::geneticalgorithm::GeneticAlgorithmBuilder>(
        *policyConfiguration, *assessment, *parameters);
}

std::optional<cloud::loadbalancer::policy::geneticalgorithm::Parameters> GeneticAlgorithmConfigurator::readParameters(
    const nlohmann::json &configuration)
{
    cloud::loadbalancer::policy::geneticalgorithm::Parameters parameters;
    try
    {
    }
    catch (nlohmann::json::out_of_range &)
    {
        return std::nullopt;
    }

    return parameters;
}

std::optional<Assessment> GeneticAlgorithmConfigurator::readAssessment(const nlohmann::json &configuration)
{
    std::string criteria;
    try
    {
        criteria = configuration.at("criteria");
    }
    catch (nlohmann::json::out_of_range &)
    {
        return std::nullopt;
    }

    if (criteria == "Makespan")
        return Assessment::Makespan;
    if (criteria == "Flowtime")
        return Assessment::Flowtime;

    return std::nullopt;
}

std::optional<PolicyConfiguration> GeneticAlgorithmConfigurator::readPolicyConfiguration(
    const nlohmann::json &configuration)
{
    std::string type;
    try
    {
        type = configuration.at("type");
    }
    catch (nlohmann::json::out_of_range &e)
    {
        return std::nullopt;
    }

    if (type == "Offline")
        return PolicyConfiguration::Offline;
    else if (type == "Online")
        return PolicyConfiguration::Online;
    else if (type == "Online with migrations and preemptions")
        return PolicyConfiguration::OnlineWithMigrationsAndPreemptions;
    else
        return std::nullopt;
}

} // namespace configuration
