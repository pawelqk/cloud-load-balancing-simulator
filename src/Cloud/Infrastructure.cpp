#include "Infrastructure.hpp"

namespace cloud
{

Infrastructure::Infrastructure(const NodeVec &nodes) : nodes(nodes)
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

} // namespace cloud
