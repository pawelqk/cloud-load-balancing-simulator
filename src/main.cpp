#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <tuple>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "Cloud/LoadBalancer/Policy/Builders/RandomBuilder.hpp"
#include "Cloud/LoadBalancer/Policy/Builders/RoundRobinBuilder.hpp"
#include "Cloud/LoadBalancer/Policy/Builders/ShortestRemainingTimeFirstBuilder.hpp"
#include "Cloud/LoadBalancer/Policy/Builders/SimulatedAnnealingBuilder.hpp"
#include "Experiment/ExperimentRunner.hpp"
#include "Instance/Instance.hpp"
#include "Logger/ResultWriter.hpp"

using json = nlohmann::json;

bool cmdOptionExists(char **begin, char **end, const std::string &option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char *argv[])
{
    std::ifstream configFile("config.json");
    if (configFile.fail())
    {
        std::cout << "Please provide config.json file\n";
        return 1;
    }

    json configuration;
    configFile >> configuration;

    const std::uint_fast64_t seed = configuration.value("seed", std::random_device{}());
    const auto algorithms = configuration.at("algorithms");
    std::vector<cloud::loadbalancer::policy::builders::PolicyBuilderPtr> policyBuilders;
    const auto policyConfiguration = instance::PolicyConfiguration::OnlineWithMigrationsAndPreemptions;
    for (auto &&algorithm : algorithms)
    {
        if (algorithm.at("name") == "Random")
            policyBuilders.push_back(
                std::make_shared<cloud::loadbalancer::policy::builders::RandomBuilder>(policyConfiguration));
        else if (algorithm.at("name") == "Round robin")
            policyBuilders.push_back(
                std::make_shared<cloud::loadbalancer::policy::builders::RoundRobinBuilder>(policyConfiguration));
        else if (algorithm.at("name") == "Shortest remaining time first")
            policyBuilders.push_back(
                std::make_shared<cloud::loadbalancer::policy::builders::ShortestRemainingTimeFirstBuilder>(
                    policyConfiguration));
        // else if (algorithm.at("name") == "Simulated annealing")
        //   policyBuilders.push_back(
        //      std::make_shared<cloud::loadbalancer::policy::builders::SimulatedAnnealingBuilder>({}));
    }

    std::ifstream instancesFile("instances.json");
    if (instancesFile.fail())
    {
        std::cout << "Please provide instances.json file\n";
        return 1;
    }

    json instancesData;
    instancesFile >> instancesData;

    std::vector<instance::Instance> instances;
    instances.reserve(instancesData.size());
    for (auto &&instanceData : instancesData)
    {
        std::map<std::uint32_t, instance::TaskDataVec> tasks;
        const auto &tasksData = instanceData.at("tasks");
        for (auto &&taskData : tasksData)
        {
            tasks[taskData.at("arrivalTime")].emplace_back(taskData.at("requiredMips"), taskData.at("length"));
        }

        const auto &instanceId = instanceData.at("id");
        const auto &nodesMips = instanceData.at("nodesMips");

        instances.emplace_back(instanceId, tasks, nodesMips);
    }

    auto resultWriter = std::make_unique<logger::ResultWriter>("results");

    experiment::ExperimentRunner::Config runnerConfig;
    runnerConfig.debug = cmdOptionExists(argv, argv + argc, "-d");
    runnerConfig.stdout = cmdOptionExists(argv, argv + argc, "--stdout");
    runnerConfig.files = cmdOptionExists(argv, argv + argc, "--files");

    experiment::ExperimentRunner runner{instances, runnerConfig, std::move(resultWriter)};
    for (auto &&policyBuilder : policyBuilders)
        runner.run(policyBuilder, seed);

    return 0;
}
