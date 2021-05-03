#include "ShortestElapsedTimeFirst.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace shortestelapsedtimefirst
{

ShortestElapsedTimeFirst::ShortestElapsedTimeFirst(const InfrastructureCPtr &infrastructure,
                                                   const logger::LoggerPtr &logger)
    : HeuristicPolicy(infrastructure, logger)
{
}

std::string ShortestElapsedTimeFirst::toString() const
{
    return "ShortestElapsedTimeFirst";
}

bool ShortestElapsedTimeFirst::heuristic(const TaskPtr &left, const TaskPtr &right) const
{
    return left->getElapsedTime() == right->getElapsedTime() ? left->getId() < right->getId()
                                                             : left->getElapsedTime() < right->getElapsedTime();
}

} // namespace shortestelapsedtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
