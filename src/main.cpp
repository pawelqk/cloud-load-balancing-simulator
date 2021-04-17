#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

#include "Cloud/CloudBuilder.hpp"
#include "Cloud/Node.hpp"
#include "Cloud/Task.hpp"
#include "Cloud/TaskImpl.hpp"
#include "Experiment/ExperimentRunner.hpp"
#include "Instance/Instance.hpp"

bool cmdOptionExists(char **begin, char **end, const std::string &option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char *argv[])
{
    instance::TaskData task1{1, 4};
    instance::TaskData task2{4, 8};
    instance::TaskData task3{2, 11};
    instance::TaskData task4{3, 12};
    instance::TaskData task5{1, 1};
    instance::TaskData task6{4, 12};
    instance::TaskData task7{2, 15};
    std::vector<std::uint32_t> nodesMips;
    std::map<std::uint32_t, instance::TaskDataVec> tasks;

    tasks.emplace(std::piecewise_construct, std::forward_as_tuple(2),
                  std::forward_as_tuple(instance::TaskDataVec{task6, task7}));
    tasks.emplace(std::piecewise_construct, std::forward_as_tuple(1),
                  std::forward_as_tuple(instance::TaskDataVec{task1, task3, task4, task5}));
    tasks.emplace(std::piecewise_construct, std::forward_as_tuple(0),
                  std::forward_as_tuple(instance::TaskDataVec{task2}));
    nodesMips.push_back(10);
    nodesMips.push_back(10);
    nodesMips.push_back(10);
    nodesMips.push_back(10);

    instance::Instance i(tasks, nodesMips);

    experiment::ExperimentRunner::Config runnerConfig;
    runnerConfig.debug = cmdOptionExists(argv, argv + argc, "-d");
    runnerConfig.stdout = cmdOptionExists(argv, argv + argc, "--stdout");
    runnerConfig.files = cmdOptionExists(argv, argv + argc, "--files");

    experiment::ExperimentRunner runner{{i}, runnerConfig};
    runner.run(cloud::Policy::Random, cloud::Assessment::Makespan);

    return 0;
}
