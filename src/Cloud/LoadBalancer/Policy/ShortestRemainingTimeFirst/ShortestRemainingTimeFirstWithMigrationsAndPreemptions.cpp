#include "ShortestRemainingTimeFirstWithMigrationsAndPreemptions.hpp"

#include <algorithm>
#include <iterator>

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace shortestremainingtimefirst
{

ShortestRemainingTimeFirstWithMigrationsAndPreemptions::ShortestRemainingTimeFirstWithMigrationsAndPreemptions(
    const InfrastructureCPtr &infrastructure, const logger::LoggerPtr &logger, const bool withMigrationsFixing)
    : HeuristicPolicyWithMigrationsAndPreemptions(infrastructure, logger, withMigrationsFixing)
{
}
std::string ShortestRemainingTimeFirstWithMigrationsAndPreemptions::toString() const
{
    return "ShortestRemainingTimeFirstWithMigrationsAndPreemptions";
}

bool ShortestRemainingTimeFirstWithMigrationsAndPreemptions::heuristic(const TaskPtr &left, const TaskPtr &right) const
{
    return left->estimateTimeLeft() == right->estimateTimeLeft() ? left->getId() < right->getId()
                                                                 : left->estimateTimeLeft() < right->estimateTimeLeft();
}

} // namespace shortestremainingtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
