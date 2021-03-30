#include "Node.hpp"

#include <iostream>

namespace cloud
{

Node::Node(const std::uint32_t id, const std::uint32_t mips) : id(id), mips(mips), logger("Node " + std::to_string(id))
{
}

void Node::assign(const Task &task)
{
    this->task.emplace(task);
}

void Node::work()
{
    if (task.has_value())
    {
        task->work();

        if (task->isDone())
            finishTask();
    }
}

Task Node::extractTask()
{
    // TODO: simple removal is probably enough
    auto extractedTask = *task;
    logger.log("%s extracted before finishing in %s", task->toString().c_str(), toString().c_str());
    task.reset();

    return extractedTask;
}

bool Node::canTaskFit(const Task &task) const
{
    return mips >= task.getMips();
}

bool Node::isIdle() const
{
    return !task.has_value();
}

std::optional<Task> Node::getTask() const
{
    return task;
}

bool Node::operator<(const Node &other) const
{
    return id < other.id;
}

bool Node::operator==(const Node &other) const
{
    return id == other.id;
}

bool Node::operator!=(const Node &other) const
{
    return !(id == other.id);
}

std::string Node::toString() const
{
    return std::string{"Node " + std::to_string(id) + "[mips: " + std::to_string(mips) + "]"};
}

void Node::finishTask()
{
    logger.log("%s finished in %s", task->toString().c_str(), toString().c_str());
    task.reset();
}

} // namespace cloud
