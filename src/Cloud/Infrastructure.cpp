#include "Infrastructure.hpp"

namespace cloud
{

Infrastructure::Infrastructure(const std::vector<std::uint32_t> &nodesMips) : nodes(prepareNodes(nodesMips))
{
}

NodeVec &Infrastructure::getNodes()
{
    return nodes;
}

const NodeVec &Infrastructure::getNodes() const
{
    return nodes;
}

TaskSet Infrastructure::advanceProcessing()
{
    TaskSet finishedTasks;
    for (auto &&node : nodes)
    {
        if (node.isIdle())
            continue;

        node.work();
        if (node.isIdle())
            finishedTasks.insert(node.extractTask());
    }

    return finishedTasks;
}

bool Infrastructure::isIdle() const
{
    return std::all_of(nodes.cbegin(), nodes.cend(), [](auto &&node) { return node.isIdle(); });
}

std::vector<Node> Infrastructure::prepareNodes(const std::vector<uint32_t> &nodesMips)
{
    std::vector<Node> nodes;
    nodes.reserve(nodesMips.size());

    NodeId nodeId{0};
    for (auto &&mips : nodesMips)
        nodes.emplace_back(nodeId++, mips);

    return nodes;
}

} // namespace cloud
