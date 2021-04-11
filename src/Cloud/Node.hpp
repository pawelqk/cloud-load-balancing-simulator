#pragma once

#include <cstdint>
#include <optional>
#include <set>
#include <vector>

#include "Logger/Logger.hpp"
#include "Task.hpp"

namespace cloud
{

using NodeId = std::uint32_t;

class Node
{
  public:
    virtual void assign(const Task &task) = 0;
    virtual void work() = 0;
    virtual Task extractTask() = 0;

    virtual bool canTaskFit(const Task &task) const = 0;
    virtual bool isIdle() const = 0;

    virtual std::optional<Task> getTask() const = 0;
    virtual NodeId getId() const = 0;

    bool operator<(const Node &other) const;
    bool operator==(const Node &other) const;
    bool operator!=(const Node &other) const;

    virtual std::string toString() const = 0;
};

using NodePtr = std::shared_ptr<Node>;
using NodePtrVec = std::vector<NodePtr>;

} // namespace cloud
