#pragma once

#include <cstdint>
#include <optional>
#include <set>

#include "Task.hpp"

namespace cloud
{

class Node
{
  public:
    explicit Node(const std::uint32_t mips);

    void assign(const Task &task);
    void work();

    bool canTaskFit(const Task &task) const;
    bool isIdle() const;

  private:
    const std::uint32_t mips;

    std::optional<Task> task;
};

} // namespace cloud
