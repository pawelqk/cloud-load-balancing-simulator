#include "ShortestElapsedTimeFirst.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
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

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
