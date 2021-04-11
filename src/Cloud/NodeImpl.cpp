#include "NodeImpl.hpp"

#include <iostream>

namespace cloud
{

NodeImpl::NodeImpl(const NodeId id, const std::uint32_t mips) : id(id), mips(mips), logger("Node " + std::to_string(id))
{
}

void NodeImpl::assign(const Task &task)
{
    this->task.emplace(task);
}

void NodeImpl::work()
{
    if (task.has_value())
    {
        task->work();
    }
}

Task NodeImpl::extractTask()
{
    auto extractedTask = *task;
    if (!task->isDone())
        logger.log("%s extracted in %s", task->toString().c_str(), toString().c_str());
    else
        logger.log("%s finished in %s", task->toString().c_str(), toString().c_str());

    task.reset();

    return extractedTask;
}

bool NodeImpl::canTaskFit(const Task &task) const
{
    return mips >= task.getMips();
}

bool NodeImpl::isIdle() const
{
    return !task.has_value() || task->isDone();
}

std::optional<Task> NodeImpl::getTask() const
{
    return task;
}

NodeId NodeImpl::getId() const
{
    return id;
}

std::string NodeImpl::toString() const
{
    return std::string{"Node " + std::to_string(id) + "[mips: " + std::to_string(mips) + "]"};
}

} // namespace cloud
