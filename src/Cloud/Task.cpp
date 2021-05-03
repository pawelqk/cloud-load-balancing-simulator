#include "Task.hpp"

#include <cmath>
#include <iostream>

namespace cloud
{

Task::~Task() = default;

bool Task::operator==(const Task &other) const
{
    return getId() == other.getId();
}

std::string Task::toString() const
{
    return std::to_string(getId());
}

void PrintTo(const TaskPtr &task, std::ostream *os)
{
    if (task != nullptr)
        *os << task->toString();
}

} // namespace cloud
