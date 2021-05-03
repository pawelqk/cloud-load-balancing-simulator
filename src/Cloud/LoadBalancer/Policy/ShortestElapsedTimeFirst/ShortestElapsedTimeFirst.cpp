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
    : PolicyBase(infrastructure, logger)
{
}

NodeToTaskMapping ShortestElapsedTimeFirst::buildNodeToTaskMapping(const TaskPtrVec &tasks)
{
    NodeToTaskMapping actions;

    return actions;
}

} // namespace shortestelapsedtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
