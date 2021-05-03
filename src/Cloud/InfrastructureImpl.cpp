#include "InfrastructureImpl.hpp"

#include <sstream>

#include "NodeImpl.hpp"

namespace cloud
{

InfrastructureImpl::InfrastructureImpl(const configuration::NodeDataVec &nodesData, const logger::LoggerPtr &logger)
    : logger(logger)
{
    prepareNodes(nodesData);
}

NodePtrVec &InfrastructureImpl::getNodes()
{
    return nodes;
}

const NodePtrVec &InfrastructureImpl::getNodes() const
{
    return nodes;
}

TaskPtrVec InfrastructureImpl::advanceProcessing()
{
    TaskPtrVec finishedTasks;
    for (auto &&node : nodes)
    {
        if (node->isIdle())
            continue;

        node->work();
        if (node->isIdle())
            finishedTasks.push_back(node->extractTask());
    }

    return finishedTasks;
}

bool InfrastructureImpl::isIdle() const
{
    return std::all_of(nodes.cbegin(), nodes.cend(), [](auto &&node) { return node->isIdle(); });
}

void InfrastructureImpl::prepareNodes(const configuration::NodeDataVec &nodesData)
{
    nodes.reserve(nodesData.size());

    for (auto &&nodeData : nodesData)
        nodes.emplace_back(std::make_shared<NodeImpl>(nodeData.id, nodeData.mips, logger));
}

} // namespace cloud
