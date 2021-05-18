#include "JsonConfigurationReader.hpp"

#include "ArtificialBeeColonyConfigurator.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBuilder.hpp"
#include "GenericConfigurator.hpp"
#include "GeneticAlgorithmConfigurator.hpp"
#include "SimulatedAnnealingConfigurator.hpp"

#include <random>

namespace configuration
{

JsonConfigurationReader::JsonConfigurationReader(const nlohmann::json &json) : json(json)
{
}

GeneralConfiguration JsonConfigurationReader::readGeneralConfiguration()
{
    GeneralConfiguration configuration;
    configuration.seed = json.value("seed", std::random_device{}());
    try
    {
        configuration.penaltyFactor = json.at("penaltyFactor");
    }
    catch (nlohmann::json::out_of_range &e)
    {
        throw InvalidConfigurationException{e.what()};
    }

    penaltyFactor = configuration.penaltyFactor;

    return configuration;
}

std::vector<cloud::loadbalancer::policy::PolicyBuilderPtr> JsonConfigurationReader::readPolicies()
{
    nlohmann::json algorithms;
    try
    {
        algorithms = json.at("algorithms");
    }
    catch (nlohmann::json::out_of_range &e)
    {
        throw InvalidConfigurationException{e.what()};
    }

    std::vector<cloud::loadbalancer::policy::PolicyBuilderPtr> policyBuilders;
    for (auto &&algorithm : algorithms)
    {
        std::string algorithmName;
        try
        {
            algorithmName = algorithm.at("name");
        }
        catch (nlohmann::json::out_of_range &e)
        {
            throw InvalidConfigurationException{e.what()};
        }

        if (const auto configurator = getAlgorithmConfigurator(algorithmName))
        {
            nlohmann::json algorithmConfiguration;
            try
            {
                algorithmConfiguration = algorithm.at("configuration");
            }
            catch (nlohmann::json::out_of_range &e)
            {
                throw InvalidConfigurationException{e.what()};
            }
            if (const auto builder = configurator->configure(algorithmConfiguration))
                policyBuilders.emplace_back(builder);
            else
                throw InvalidConfigurationException{"Invalid configuration for " + algorithmName};
        }
        else
            throw InvalidConfigurationException{"Unknown algorithm: " + algorithmName};
    }

    return policyBuilders;
}

JsonAlgorithmConfiguratorPtr JsonConfigurationReader::getAlgorithmConfigurator(const std::string &algorithmName)
{
    if (algorithmName == "Artificial bee colony")
        return std::make_unique<ArtificialBeeColonyConfigurator>();
    if (algorithmName == "First come first serve")
        return std::make_unique<GenericConfigurator>(Policy::FirstComeFirstServe);
    if (algorithmName == "Genetic algorithm")
        return std::make_unique<GeneticAlgorithmConfigurator>();
    if (algorithmName == "Longest remaining time first")
        return std::make_unique<GenericConfigurator>(Policy::LongestRemainingTimeFirst);
    if (algorithmName == "Random")
        return std::make_unique<GenericConfigurator>(Policy::Random);
    if (algorithmName == "Round robin")
        return std::make_unique<GenericConfigurator>(Policy::RoundRobin);
    if (algorithmName == "Shortest elapsed time first")
        return std::make_unique<GenericConfigurator>(Policy::ShortestElapsedTimeFirst);
    if (algorithmName == "Shortest remaining time first")
        return std::make_unique<GenericConfigurator>(Policy::ShortestRemainingTimeFirst);
    if (algorithmName == "Simulated annealing")
        return std::make_unique<SimulatedAnnealingConfigurator>(penaltyFactor);

    return nullptr;
}

} // namespace configuration
