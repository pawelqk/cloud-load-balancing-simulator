#include "LongestRemainingTimeFirstWithMigrationsAndPreemptions.hpp"

#include <algorithm>
#include <iterator>

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace longestremainingtimefirst
{

LongestRemainingTimeFirstWithMigrationsAndPreemptions::LongestRemainingTimeFirstWithMigrationsAndPreemptions(
    const InfrastructureCPtr &infrastructure, const logger::LoggerPtr &logger, const bool withMigrationsFixing)
    : HeuristicPolicyWithMigrationsAndPreemptions(infrastructure, logger, withMigrationsFixing)
{
}
std::string LongestRemainingTimeFirstWithMigrationsAndPreemptions::toString() const
{
    return "LongestRemainingTimeFirstWithMigrationsAndPreemptions";
}

bool LongestRemainingTimeFirstWithMigrationsAndPreemptions::heuristic(const TaskPtr &left, const TaskPtr &right) const
{
    return left->estimateTimeLeft() == right->estimateTimeLeft() ? left->getId() < right->getId()
                                                                 : left->estimateTimeLeft() > right->estimateTimeLeft();
}

} // namespace longestremainingtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
