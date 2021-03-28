#include "Task.hpp"

namespace cloud
{

Task::Task(const std::uint32_t mips, const std::uint32_t initialLength) : mips(mips), initialLength(initialLength)
{
}

void Task::work(const std::uint32_t instructions)
{
    if (instructions >= length)
        length = 0;
    else
        length -= instructions;
}

bool Task::isDone() const
{
    return length == 0;
}

std::uint32_t Task::getMips() const
{
    return mips;
}

} // namespace cloud
