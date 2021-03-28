#include "Node.hpp"

namespace cloud
{

Node::Node(const std::uint32_t mips) : mips(mips)
{
}

void Node::assign(const Task &task)
{
    task = task;
}

void Node::work()
{
    if (task.has_value())
    {
        task->work(mips);

        if (task->isDone())
            task.reset();
    }
}

bool Node::canTaskFit(const Task &task) const
{
    return mips >= task.getMips();
}

bool Node::isIdle() const
{
    return !task.has_value();
}

} // namespace cloud
