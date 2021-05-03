#include "ShortestElapsedTimeFirstWithMigrationsAndPreemptions.hpp"

#include <algorithm>
#include <iterator>

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace shortestelapsedtimefirst
{

ShortestElapsedTimeFirstWithMigrationsAndPreemptions::ShortestElapsedTimeFirstWithMigrationsAndPreemptions(
    const InfrastructureCPtr &infrastructure, const logger::LoggerPtr &logger, const bool withMigrationsFixing)
    : HeuristicPolicyWithMigrationsAndPreemptions(infrastructure, logger, withMigrationsFixing)
{
}

std::string ShortestElapsedTimeFirstWithMigrationsAndPreemptions::toString() const
{
    return "ShortestElapsedTimeFirstWithMigrationsAndPreemptions";
}

bool ShortestElapsedTimeFirstWithMigrationsAndPreemptions::heuristic(const TaskPtr &left, const TaskPtr &right) const
{
    return left->getElapsedTime() == right->getElapsedTime() ? left->getId() < right->getId()
                                                             : left->getElapsedTime() < right->getElapsedTime();
}

} // namespace shortestelapsedtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
