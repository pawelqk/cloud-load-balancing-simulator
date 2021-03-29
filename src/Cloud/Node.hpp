#pragma once

#include <cstdint>
#include <optional>
#include <set>
#include <vector>

#include "Task.hpp"

namespace cloud
{

class Node
{
  public:
    explicit Node(const std::uint32_t id, const std::uint32_t mips);

    void assign(const Task &task);
    void work();

    bool canTaskFit(const Task &task) const;
    bool isIdle() const;

    bool operator<(const Node &other) const;
    bool operator==(const Node &other) const;

  private:
    const std::uint32_t id;
    const std::uint32_t mips;

    std::optional<Task> task;
};

using NodeSet = std::set<Node>;
using NodeVec = std::vector<Node>;

} // namespace cloud
