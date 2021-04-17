#include "Node.hpp"

#include <iostream>

namespace cloud
{

Node::~Node() = default;

bool Node::operator<(const Node &other) const
{
    return getId() < other.getId();
}

bool Node::operator==(const Node &other) const
{
    return getId() == other.getId();
}

bool Node::operator!=(const Node &other) const
{
    return !(getId() == other.getId());
}

} // namespace cloud
