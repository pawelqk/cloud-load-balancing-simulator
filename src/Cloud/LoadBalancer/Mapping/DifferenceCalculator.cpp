#include "DifferenceCalculator.hpp"

#include <utility>

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{

bool operator==(const Migration &lhs, const Migration &rhs)
{
    return std::tie(lhs.source, lhs.destination, lhs.task) == std::tie(rhs.source, rhs.destination, rhs.task);
}

bool operator==(const Preemption &lhs, const Preemption &rhs)
{
    return std::tie(lhs.source, lhs.task) == std::tie(rhs.source, rhs.task);
}

bool operator==(const Difference &lhs, const Difference &rhs)
{
    return std::tie(lhs.migrations, lhs.preemptions, lhs.nodeIdsWithoutChange) ==
           std::tie(rhs.migrations, rhs.preemptions, rhs.nodeIdsWithoutChange);
}

DifferenceCalculator::~DifferenceCalculator() = default;

} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
