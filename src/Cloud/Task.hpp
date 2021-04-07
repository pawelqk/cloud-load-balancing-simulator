#pragma once

#include <cstdint>
#include <ostream>
#include <set>

namespace cloud
{

class Task
{
  public:
    explicit Task(const std::uint32_t id, const std::uint32_t mips, const std::uint32_t initialLength);
    ~Task();

    void work();
    bool isDone() const;

    std::uint32_t getId() const;
    std::uint32_t getMips() const;
    std::uint32_t estimateTimeLeft() const;

    bool operator<(const Task &other) const;
    bool operator==(const Task &other) const;

    std::string toString() const;

  private:
    const std::uint32_t id;
    const std::uint32_t mips;
    const std::uint32_t initialLength;

    std::uint32_t length;
};

using TaskSet = std::set<Task>;

} // namespace cloud
