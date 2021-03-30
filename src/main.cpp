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
    cloud::Task task2(1, 2, 5);
    cloud::Task task3(2, 2, 11);
    cloud::Task task4(3, 2, 12);
    cloud::Task task5(4, 1, 1);
    cloud::Node node1(0, 2);
    cloud::Node node2(1, 1);
    std::vector<cloud::Node> nodes;
    std::map<std::uint32_t, std::vector<cloud::Task>> tasks;

    tasks.emplace(std::piecewise_construct, std::forward_as_tuple(2),
                  std::forward_as_tuple(std::vector<cloud::Task>{task1, task3, task4, task5}));
    tasks.emplace(std::piecewise_construct, std::forward_as_tuple(0),
                  std::forward_as_tuple(std::vector<cloud::Task>{task2}));
    nodes.push_back(node1);
    nodes.push_back(node2);

    instance::Instance i(tasks, nodes);

    experiment::ExperimentRunner runner{{i}};
    runner.run();

    return 0;
}
