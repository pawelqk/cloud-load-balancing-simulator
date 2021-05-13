#include "ArtificialBeeColonyConfigurator.hpp"

#include "Cloud/LoadBalancer/Policy/ArtificialBeeColony/ArtificialBeeColonyBuilder.hpp"

namespace configuration
{

cloud::loadbalancer::policy::PolicyBuilderPtr ArtificialBeeColonyConfigurator::configure(
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

    return std::make_shared<cloud::loadbalancer::policy::artificialbeecolony::ArtificialBeeColonyBuilder>(
        *policyConfiguration, *assessment, *parameters);
}

std::optional<cloud::loadbalancer::policy::artificialbeecolony::Parameters> ArtificialBeeColonyConfigurator::
    readParameters(const nlohmann::json &configuration)
{
    cloud::loadbalancer::policy::artificialbeecolony::Parameters parameters;
    try
    {
    }
    catch (nlohmann::json::out_of_range &)
    {
        return std::nullopt;
    }

    return parameters;
}

std::optional<Assessment> ArtificialBeeColonyConfigurator::readAssessment(const nlohmann::json &configuration)
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

std::optional<PolicyConfiguration> ArtificialBeeColonyConfigurator::readPolicyConfiguration(
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
