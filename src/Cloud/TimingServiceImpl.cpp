#include "TimingServiceImpl.hpp"

namespace cloud
{

TimingServiceImpl::TimingServiceImpl(const logger::LoggerPtr &logger) : logger(logger), ticks(0), totalFlowtime(0)
{
}

void TimingServiceImpl::tick()
{
    ++ticks;
    logger->debug("Tick %u", ticks);
}

void TimingServiceImpl::addFlowtime(const std::uint32_t flowtime)
{
    totalFlowtime += flowtime;
}

std::uint32_t TimingServiceImpl::getTicks() const
{
    return ticks;
}

std::uint32_t TimingServiceImpl::getTotalFlowtime() const
{
    return totalFlowtime;
}

} // namespace cloud
