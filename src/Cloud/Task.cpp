#include "Task.hpp"

namespace cloud
{

Task::Task(const std::uint32_t id, const std::uint32_t mips, const std::uint32_t initialLength)
    : id(id), mips(mips), initialLength(initialLength), length(initialLength)
{
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

std::uint32_t Task::getMips() const
{
    return mips;
}

bool Task::operator<(const Task &other) const
{
    return id < other.id;
}

} // namespace cloud
