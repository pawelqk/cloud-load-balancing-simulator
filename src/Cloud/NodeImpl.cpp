#include "NodeImpl.hpp"

#include <iostream>

namespace cloud
{

NodeImpl::NodeImpl(const NodeId id, const std::uint32_t mips, const logger::LoggerPtr &logger)
    : id(id), mips(mips), logger(logger), task(nullptr)
{
}

void NodeImpl::assign(const TaskPtr &task)
{
    this->task = task;
}

void NodeImpl::work()
{
    if (task != nullptr)
        task->work();
}

TaskPtr NodeImpl::extractTask()
{
    auto extractedTask = task;
    if (!task->isDone())
        logger->debug("%s extracted in %s", task->toString().c_str(), toString().c_str());
    else
        logger->debug("[TASK FINISH] %s finished in %s", task->toString().c_str(), toString().c_str());

    task.reset();

    return extractedTask;
}

bool NodeImpl::canTaskFit(const TaskPtr &task) const
{
    return mips >= task->getMips();
}

bool NodeImpl::isIdle() const
{
    return task == nullptr || task->isDone();
}

TaskPtr NodeImpl::getTask() const
{
    return task;
}

NodeId NodeImpl::getId() const
{
    return id;
}

std::uint32_t NodeImpl::getMips() const
{
    return mips;
}

std::string NodeImpl::toString() const
{
    return std::string{"Node " + std::to_string(id) + "[mips: " + std::to_string(mips) + "]"};
}

} // namespace cloud
