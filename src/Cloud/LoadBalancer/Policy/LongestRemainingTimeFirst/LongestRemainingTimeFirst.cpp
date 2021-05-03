#include "LongestRemainingTimeFirst.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace longestremainingtimefirst
{

LongestRemainingTimeFirst::LongestRemainingTimeFirst(const InfrastructureCPtr &infrastructure,
                                                     const logger::LoggerPtr &logger)
    : HeuristicPolicy(infrastructure, logger)
{
}

std::string LongestRemainingTimeFirst::toString() const
{
    return "LongestRemainingTimeFirst";
}

bool LongestRemainingTimeFirst::heuristic(const TaskPtr &left, const TaskPtr &right) const
{
    return left->estimateTimeLeft() == right->estimateTimeLeft() ? left->getId() < right->getId()
                                                                 : left->estimateTimeLeft() > right->estimateTimeLeft();
}

} // namespace longestremainingtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
