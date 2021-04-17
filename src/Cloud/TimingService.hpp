#pragma once

#include <cstdint>
#include <memory>

namespace cloud
{

class TimingService
{
  public:
    virtual ~TimingService() = default;

    virtual void tick() = 0;
    virtual void addFlowtime(const std::uint32_t) = 0;

    virtual std::uint32_t getTicks() const = 0;
    virtual std::uint32_t getTotalFlowtime() const = 0;
};

using TimingServicePtr = std::shared_ptr<TimingService>;

} // namespace cloud
