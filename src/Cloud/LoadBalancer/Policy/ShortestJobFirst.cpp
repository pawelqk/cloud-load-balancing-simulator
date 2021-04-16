#include "ShortestJobFirst.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

ShortestJobFirst::ShortestJobFirst(const InfrastructureCPtr &infrastructure) : PolicyBase(infrastructure)
{
}

MappingActions ShortestJobFirst::buildTaskToNodeMapping(const TaskSet &tasks)
{
    MappingActions actions;

    return actions;
}

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
