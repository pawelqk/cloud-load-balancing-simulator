#include "ShortestRemainingTimeFirst.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace shortestremainingtimefirst
{

ShortestRemainingTimeFirst::ShortestRemainingTimeFirst(const InfrastructureCPtr &infrastructure,
                                                       const logger::LoggerPtr &logger)
    : HeuristicPolicy(infrastructure, logger)
{
}

std::string ShortestRemainingTimeFirst::toString() const
{
    return "ShortestRemainingTimeFirst";
}

bool ShortestRemainingTimeFirst::heuristic(const TaskPtr &left, const TaskPtr &right) const
{
    return left->estimateTimeLeft() == right->estimateTimeLeft() ? left->getId() < right->getId()
                                                                 : left->estimateTimeLeft() < right->estimateTimeLeft();
}

} // namespace shortestremainingtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
