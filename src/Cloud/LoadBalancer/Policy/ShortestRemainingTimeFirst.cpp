#include "ShortestRemainingTimeFirst.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

ShortestRemainingTimeFirst::ShortestRemainingTimeFirst(const InfrastructureCPtr &infrastructure)
    : PolicyBase(infrastructure)
{
}

MappingActions ShortestRemainingTimeFirst::buildTaskToNodeMapping(const TaskSet &tasks)
{
    MappingActions actions;

    return actions;
}

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
