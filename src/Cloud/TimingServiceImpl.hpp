#pragma once

#include <cstdint>

#include "Logger/Logger.hpp"
#include "TimingService.hpp"

namespace cloud
{

class TimingServiceImpl : public TimingService
{
  public:
    TimingServiceImpl(const logger::LoggerPtr &logger);

    void tick() override;
    void addFlowtime(const std::uint32_t flowtime) override;

    std::uint32_t getTicks() const override;
    std::uint32_t getTotalFlowtime() const override;

  private:
    const logger::LoggerPtr logger;
    std::uint32_t ticks;
    std::uint32_t totalFlowtime;
};

} // namespace cloud
