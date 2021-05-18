#include "SimulatedAnnealingConfigurator.hpp"

#include "Cloud/LoadBalancer/Policy/SimulatedAnnealing/SimulatedAnnealingBase.hpp"
#include "Cloud/LoadBalancer/Policy/SimulatedAnnealing/SimulatedAnnealingBuilder.hpp"

namespace configuration
{

SimulatedAnnealingConfigurator::SimulatedAnnealingConfigurator(const double penaltyFactor)
    : penaltyFactor(penaltyFactor)
{
}

cloud::loadbalancer::policy::PolicyBuilderPtr SimulatedAnnealingConfigurator::configure(
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

    return std::make_shared<cloud::loadbalancer::policy::simulatedannealing::SimulatedAnnealingBuilder>(
        *policyConfiguration, *assessment, *parameters, penaltyFactor);
}

std::optional<cloud::loadbalancer::policy::simulatedannealing::Parameters> SimulatedAnnealingConfigurator::
    readParameters(const nlohmann::json &configuration)
{
    cloud::loadbalancer::policy::simulatedannealing::Parameters parameters;
    try
    {
        parameters.coolingRatio = configuration.at("coolingRatio");
        parameters.startTemperature = configuration.at("startTemperature");
        parameters.endTemperature = configuration.at("endTemperature");
        parameters.iterationsPerStep = configuration.at("iterationsPerStep");
        parameters.maxIterationsWithoutChange = configuration.at("maxIterationsWithoutChange");
    }
    catch (nlohmann::json::out_of_range &)
    {
        return std::nullopt;
    }

    std::string initialPopulationGenerationMethod;
    try
    {
        initialPopulationGenerationMethod = configuration.at("initialPopulationGenerationMethod");
    }
    catch (nlohmann::json::out_of_range &e)
    {
        return std::nullopt;
    }

    if (initialPopulationGenerationMethod == "Random")
        parameters.initialPopulationGenerationMethod =
            cloud::loadbalancer::policy::simulatedannealing::InitialPopulationGenerationMethod::Random;
    else if (initialPopulationGenerationMethod == "SRTF")
        parameters.initialPopulationGenerationMethod =
            cloud::loadbalancer::policy::simulatedannealing::InitialPopulationGenerationMethod::SRTF;
    else
        return std::nullopt;

    return parameters;
}

std::optional<Assessment> SimulatedAnnealingConfigurator::readAssessment(const nlohmann::json &configuration)
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

std::optional<PolicyConfiguration> SimulatedAnnealingConfigurator::readPolicyConfiguration(
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
