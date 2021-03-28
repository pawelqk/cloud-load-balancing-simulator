#pragma once

#include <cstdint>
#include <optional>

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
    std::optional<Task> task;
    const std::uint32_t mips;
};

} // namespace cloud
