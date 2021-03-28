#include <iostream>
#include <vector>

#include "Cloud/Node.hpp"
#include "Cloud/Task.hpp"
#include "Experiment/ExperimentRunner.hpp"
#include "Instance/Instance.hpp"

int main()
{
    cloud::Task task1(0, 1, 4);
    cloud::Task task2(1, 2, 5);
    cloud::Node node1(2);
    cloud::Node node2(1);
    std::vector<cloud::Node> nodes;
    std::vector<cloud::Task> tasks;

    tasks.push_back(task1);
    tasks.push_back(task2);
    nodes.push_back(node2);
    nodes.push_back(node1);

    instance::Instance i(tasks, nodes);

    experiment::ExperimentRunner runner{{i}};
    runner.run();

    return 0;
}
