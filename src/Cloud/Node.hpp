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
    explicit Node(const NodeId id, const std::uint32_t mips);
    Node() = delete;
    Node(const Node &) = delete;
    Node(Node &&) = default;
    Node &operator=(const Node &) = delete;
    Node &operator=(Node &&) = delete;

    void assign(const Task &task);
    void work();
    Task extractTask();

    bool canTaskFit(const Task &task) const;
    bool isIdle() const;

    std::optional<Task> getTask() const;
    NodeId getId() const;

    bool operator<(const Node &other) const;
    bool operator==(const Node &other) const;
    bool operator!=(const Node &other) const;

    std::string toString() const;

  private:
    const NodeId id;
    const std::uint32_t mips;

    std::optional<Task> task;
    logger::Logger logger;
};

using NodeSet = std::set<Node>;
using NodeVec = std::vector<Node>;

} // namespace cloud
