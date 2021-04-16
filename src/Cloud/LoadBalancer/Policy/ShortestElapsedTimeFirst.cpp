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

MappingActions ShortestElapsedTimeFirst::buildTaskToNodeMapping(const TaskSet &tasks)
{
    MappingActions actions;

    return actions;
}

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
