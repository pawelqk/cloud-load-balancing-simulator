#include "ShortestElapsedTimeFirst.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

ShortestElapsedTimeFirst::ShortestElapsedTimeFirst(const InfrastructureCPtr &infrastructure)
    : PolicyBase(infrastructure)
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
