#include "TaskImpl.hpp"

#include <cmath>
#include <iostream>

namespace
{

constexpr auto PENALTY_FACTOR = 0.2;

}

namespace cloud
{

TaskImpl::TaskImpl(const std::uint32_t id, const std::uint32_t mips, const std::uint32_t initialLength,
                   const std::uint32_t arrivalTime)
    : id(id), mips(mips), initialLength(initialLength), arrivalTime(arrivalTime), currentLength(initialLength)
{
}

void TaskImpl::work()
{
    if (mips >= currentLength)
        currentLength = 0;
    else
        currentLength -= mips;
}

bool TaskImpl::isDone() const
{
    return currentLength == 0;
}

void TaskImpl::performPreemption()
{
    currentLength = initialLength;
}

void TaskImpl::performMigration()
{
    currentLength += calculateMigrationPenalty();
}

std::uint32_t TaskImpl::getId() const
{
    return mips;
}

std::uint32_t TaskImpl::getMips() const
{
    return mips;
}

std::uint32_t TaskImpl::getInitialLength() const
{
    return initialLength;
}

std::uint32_t TaskImpl::getArrivalTime() const
{
    return arrivalTime;
}

std::uint32_t TaskImpl::estimateTimeLeft() const
{
    return estimateTime(currentLength);
}

std::uint32_t TaskImpl::estimateTimeLeftAfterMigration() const
{
    return estimateTime(currentLength + calculateMigrationPenalty());
}

std::uint32_t TaskImpl::estimateTimeLeftAfterPreemption() const
{
    return estimateTime(initialLength);
}

std::string TaskImpl::toString() const
{
    return std::string{"Task " + std::to_string(id) + "[length: " + std::to_string(initialLength) +
                       ", mips: " + std::to_string(mips) + ", arrival: " + std::to_string(arrivalTime) + "]"};
}

std::uint32_t TaskImpl::calculateMigrationPenalty() const
{
    return std::ceil(initialLength * PENALTY_FACTOR);
}

std::uint32_t TaskImpl::estimateTime(const std::uint32_t lengthToEstimate) const
{
    return std::ceil(static_cast<double>(lengthToEstimate) / mips);
}

} // namespace cloud
