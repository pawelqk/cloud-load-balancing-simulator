#include "InfrastructureImpl.hpp"
#include "NodeImpl.hpp"

namespace cloud
{

InfrastructureImpl::InfrastructureImpl(const std::vector<std::uint32_t> &nodesMips, const logger::LoggerPtr &logger)
    : nodes(prepareNodes(nodesMips, logger))
{
}

NodePtrVec &InfrastructureImpl::getNodes()
{
    return nodes;
}

const NodePtrVec &InfrastructureImpl::getNodes() const
{
    return nodes;
}

TaskSet InfrastructureImpl::advanceProcessing()
{
    TaskSet finishedTasks;
    for (auto &&node : nodes)
    {
        if (node->isIdle())
            continue;

        node->work();
        if (node->isIdle())
            finishedTasks.insert(node->extractTask());
    }

    return finishedTasks;
}

bool InfrastructureImpl::isIdle() const
{
    return std::all_of(nodes.cbegin(), nodes.cend(), [](auto &&node) { return node->isIdle(); });
}

NodePtrVec InfrastructureImpl::prepareNodes(const std::vector<uint32_t> &nodesMips, const logger::LoggerPtr &logger)
{
    NodePtrVec nodes;
    nodes.reserve(nodesMips.size());

    NodeId nodeId{0};
    for (auto &&mips : nodesMips)
        nodes.emplace_back(std::make_shared<NodeImpl>(nodeId++, mips, logger));

    return nodes;
}

} // namespace cloud
