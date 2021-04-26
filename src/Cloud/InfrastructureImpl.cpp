#include "InfrastructureImpl.hpp"

#include <sstream>

#include "NodeImpl.hpp"

namespace cloud
{

InfrastructureImpl::InfrastructureImpl(const std::vector<std::uint32_t> &nodesMips, const logger::LoggerPtr &logger)
    : logger(logger)
{
    prepareNodes(nodesMips);
    logger->info("Nodes mapped: %s", toString().c_str());
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

void InfrastructureImpl::prepareNodes(const std::vector<uint32_t> &nodesMips)
{
    nodes.reserve(nodesMips.size());

    NodeId nodeId{0};
    for (auto &&mips : nodesMips)
        nodes.emplace_back(std::make_shared<NodeImpl>(nodeId++, mips, logger));
}

std::string InfrastructureImpl::toString() const
{
    std::stringstream ss;

    ss << "[";

    for (auto i = 0u; i < nodes.size() - 1; ++i)
        ss << "(id: " << nodes[i]->getId() << ", mips: " << nodes[i]->getMips() << "), ";

    const auto &lastNode = nodes[nodes.size() - 1];
    ss << "(id: " << lastNode->getId() << ", mips: " << lastNode->getMips() << ")";

    ss << "]";

    return ss.str();
}

} // namespace cloud
