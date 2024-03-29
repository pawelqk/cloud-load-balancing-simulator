#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <tuple>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "Cloud/LoadBalancer/Policy/Random/RandomBuilder.hpp"
#include "Cloud/LoadBalancer/Policy/RoundRobin/RoundRobinBuilder.hpp"
#include "Cloud/LoadBalancer/Policy/ShortestRemainingTimeFirst/ShortestRemainingTimeFirstBuilder.hpp"
#include "Cloud/LoadBalancer/Policy/SimulatedAnnealing/SimulatedAnnealingBuilder.hpp"
#include "Configuration/ConfigurationReader.hpp"
#include "Configuration/Instance.hpp"
#include "Configuration/JsonConfigurationReader.hpp"
#include "Configuration/JsonInstancesReader.hpp"
#include "Experiment/ExperimentRunner.hpp"
#include "Logger/CsvResultWriter.hpp"

using json = nlohmann::json;

bool cmdOptionExists(char **begin, char **end, const std::string &option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char *argv[])
{
    std::ifstream configFile("run_config.json");
    if (configFile.fail())
    {
        std::cout << "Please provide config.json file\n";
        return 1;
    }

    json configuration;
    configFile >> configuration;

    configuration::JsonConfigurationReader configurationReader{configuration};

    configuration::GeneralConfiguration generalConfiguration;
    std::vector<cloud::loadbalancer::policy::PolicyBuilderPtr> policyBuilders;
    try
    {
        generalConfiguration = configurationReader.readGeneralConfiguration();
        policyBuilders = configurationReader.readPolicies();
    }
    catch (configuration::InvalidConfigurationException &ex)
    {
        std::cout << ex.what();
        return 1;
    }

    std::ifstream instancesFile("instances.json");
    if (instancesFile.fail())
    {
        std::cout << "Please provide instances.json file\n";
        return 1;
    }

    json instancesData;
    instancesFile >> instancesData;
    auto instanceReader = std::make_unique<configuration::JsonInstancesReader>(instancesData);

    std::vector<configuration::Instance> instances;
    try
    {
        instances = instanceReader->read();
    }
    catch (configuration::InvalidConfigurationException &ex)
    {
        std::cout << ex.what();
        return 1;
    }

    auto resultWriter = std::make_unique<logger::CsvResultWriter>("results");

    experiment::ExperimentRunner::Config runnerConfig;
    runnerConfig.debug = cmdOptionExists(argv, argv + argc, "-d");
    runnerConfig.stdout = cmdOptionExists(argv, argv + argc, "--stdout");
    runnerConfig.files = cmdOptionExists(argv, argv + argc, "--files");

    experiment::ExperimentRunner runner{instances, runnerConfig, std::move(resultWriter)};
    for (auto &&policyBuilder : policyBuilders)
        runner.run(policyBuilder, generalConfiguration);

    return 0;
}
