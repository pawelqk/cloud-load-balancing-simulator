#include "FirstComeFirstServe.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace firstcomefirstserve
{

FirstComeFirstServe::FirstComeFirstServe(const InfrastructureCPtr &infrastructure, const logger::LoggerPtr &logger)
    : HeuristicPolicy(infrastructure, logger)
{
}

std::string FirstComeFirstServe::toString() const
{
    return "FirstComeFirstServe";
}

bool FirstComeFirstServe::heuristic(const TaskPtr &left, const TaskPtr &right) const
{
    return left->getArrivalTime() == right->getArrivalTime() ? left->getId() < right->getId()
                                                             : left->getArrivalTime() < right->getArrivalTime();
}

} // namespace firstcomefirstserve
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
