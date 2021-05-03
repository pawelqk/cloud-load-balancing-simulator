#include "TaskImpl.hpp"

#include <cmath>
#include <iostream>

namespace cloud
{

TaskImpl::TaskImpl(const std::uint32_t id, const std::uint32_t mips, const std::uint32_t initialLength,
                   const std::uint32_t arrivalTime, const double penaltyFactor)
    : id(id), mips(mips), initialLength(initialLength), arrivalTime(arrivalTime), penaltyFactor(penaltyFactor),
      currentLength(initialLength), elapsedTime(0)
{
}

void TaskImpl::work()
{
    if (mips >= currentLength)
        currentLength = 0;
    else
        currentLength -= mips;

    ++elapsedTime;
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
    return id;
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

std::uint32_t TaskImpl::getElapsedTime() const
{
    return elapsedTime;
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
    return std::ceil(initialLength * penaltyFactor);
}

std::uint32_t TaskImpl::estimateTime(const std::uint32_t lengthToEstimate) const
{
    return std::ceil(static_cast<double>(lengthToEstimate) / mips);
}

} // namespace cloud
