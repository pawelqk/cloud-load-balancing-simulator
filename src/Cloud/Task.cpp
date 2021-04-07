#include "Task.hpp"

#include <cmath>
#include <iostream>

namespace cloud
{

Task::Task(const std::uint32_t id, const std::uint32_t mips, const std::uint32_t initialLength)
    : id(id), mips(mips), initialLength(initialLength), length(initialLength)
{
}

Task::~Task()
{
    // std::cout << "Task " << id << " done.\n";
}

void Task::work()
{
    if (mips >= length)
        length = 0;
    else
        length -= mips;
}

bool Task::isDone() const
{
    return length == 0;
}

std::uint32_t Task::getId() const
{
    return mips;
}

std::uint32_t Task::getMips() const
{
    return mips;
}

std::uint32_t Task::estimateTimeLeft() const
{
    return std::ceil(static_cast<double>(length) / mips);
}

bool Task::operator<(const Task &other) const
{
    return id < other.id;
}

bool Task::operator==(const Task &other) const
{
    return id == other.id;
}

std::string Task::toString() const
{
    return std::string{"Task " + std::to_string(id) + "[length: " + std::to_string(initialLength) +
                       ", mips: " + std::to_string(mips) + "]"};
}

} // namespace cloud
