#include <iostream>
#include <vector>

#include "Cloud/Node.hpp"
#include "Cloud/Task.hpp"
#include "Instance/Instance.hpp"

int main()
{
    cloud::Task task1(1, 4);
    cloud::Task task2(2, 5);
    cloud::Node node(2);
    std::vector<cloud::Node> nodes;
    std::vector<cloud::Task> tasks;

    tasks.push_back(task1);
    tasks.push_back(task2);
    nodes.push_back(node);

    instance::Instance i(tasks, nodes);

    return 0;
}
