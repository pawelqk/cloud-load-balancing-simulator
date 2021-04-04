#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

#include "Cloud/Node.hpp"
#include "Cloud/Task.hpp"
#include "Experiment/ExperimentRunner.hpp"
#include "Instance/Instance.hpp"

int main()
{
    cloud::Task task1(0, 1, 4);
    cloud::Task task2(1, 1, 1);
    cloud::Task task3(2, 2, 11);
    cloud::Task task4(3, 2, 12);
    cloud::Task task5(4, 1, 1);
    std::vector<std::uint32_t> nodesMips;
    std::map<std::uint32_t, std::vector<cloud::Task>> tasks;

    tasks.emplace(std::piecewise_construct, std::forward_as_tuple(2),
                  std::forward_as_tuple(std::vector<cloud::Task>{task1, task3, task4, task5}));
    tasks.emplace(std::piecewise_construct, std::forward_as_tuple(0),
                  std::forward_as_tuple(std::vector<cloud::Task>{task2}));
    nodesMips.push_back(1);
    nodesMips.push_back(5);

    instance::Instance i(tasks, nodesMips);

    experiment::ExperimentRunner runner{{i}};
    runner.run();

    return 0;
}
