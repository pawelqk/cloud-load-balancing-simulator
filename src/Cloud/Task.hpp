#pragma once

#include <cstdint>

namespace cloud
{

class Task
{
  public:
    Task(const std::uint32_t mips, const std::uint32_t initialLength);

    void work(const std::uint32_t instructions);
    bool isDone() const;

    std::uint32_t getMips() const;

  private:
    const std::uint32_t mips;
    const std::uint32_t initialLength;

    std::uint32_t length;
};

} // namespace cloud
