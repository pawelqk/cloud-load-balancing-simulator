#pragma once

#include <cstdint>
#include <set>

namespace cloud
{

class Task
{
  public:
    Task(const std::uint32_t id, const std::uint32_t mips, const std::uint32_t initialLength);

    void work();
    bool isDone() const;

    std::uint32_t getMips() const;

    bool operator<(const Task &other) const;

  private:
    const std::uint32_t id;
    const std::uint32_t mips;
    const std::uint32_t initialLength;

    std::uint32_t length;
};

using TaskSet = std::set<Task>;

} // namespace cloud
