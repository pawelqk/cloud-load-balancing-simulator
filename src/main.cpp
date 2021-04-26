#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <tuple>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "Cloud/CloudBuilder.hpp"
#include "Cloud/Node.hpp"
#include "Cloud/Task.hpp"
#include "Cloud/TaskImpl.hpp"
#include "Experiment/ExperimentRunner.hpp"
#include "Instance/Instance.hpp"

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
    std::vector<cloud::Policy> policies;
    for (auto &&algorithm : algorithms)
    {
        if (algorithm.at("name") == "Random")
            policies.push_back(cloud::Policy::Random);
        else if (algorithm.at("name") == "Round robin")
            policies.push_back(cloud::Policy::RoundRobin);
        else if (algorithm.at("name") == "Simulated annealing")
            policies.push_back(cloud::Policy::SimulatedAnnealing);
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

    experiment::ExperimentRunner::Config runnerConfig;
    runnerConfig.debug = cmdOptionExists(argv, argv + argc, "-d");
    runnerConfig.stdout = cmdOptionExists(argv, argv + argc, "--stdout");
    runnerConfig.files = cmdOptionExists(argv, argv + argc, "--files");

    experiment::ExperimentRunner runner{instances, runnerConfig};
    for (auto &&policy : policies)
        runner.run(policy, cloud::Assessment::Makespan, seed);

    return 0;
}
