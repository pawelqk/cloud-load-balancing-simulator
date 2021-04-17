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

} // namespace cloud
