#include "OfflineHeuristicPolicy.hpp"
#include "Configuration/Instance.hpp"
#include <cstdint>

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

OfflineHeuristicPolicy::OfflineHeuristicPolicy(const InfrastructurePtr &infrastructure, const logger::LoggerPtr &logger,
                                               const configuration::Instance &instance)
    : HeuristicPolicyWithMigrationsAndPreemptions(infrastructure, logger, false),
      offlineProblemAdapter(infrastructure, buildSolution(infrastructure, instance))
{
}

NodeToTaskMapping OfflineHeuristicPolicy::buildNodeToTaskMapping(const TaskPtrVec &tasks)
{
    return offlineProblemAdapter.getNextMapping(tasks);
}

NodeToTaskMapping OfflineHeuristicPolicy::buildSolution(const InfrastructurePtr &infrastructure,
                                                        const configuration::Instance &instance)
{
    // TODO:
    // Online solution == offline solution when migrations and preemptions are not used so the only way to have offline
    // improved is to use them. This requires keeping track of state of the nodes at all times and is kind of difficult
    // to do with current implementation
    return {};
}

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
