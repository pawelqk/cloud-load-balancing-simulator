#include "Experiment.hpp"

#include <cstdint>
#include <iostream>

#include "Cloud/Cloud.hpp"
#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/LoadBalancerImpl.hpp"
#include "Cloud/LoadBalancer/Strategy/RoundRobin.hpp"

namespace experiment
{

Experiment::Experiment(const instance::Instance &instance) : instance(instance), logger("Experiment")
{
}

void Experiment::run()
{
    const auto infrastructure = std::make_shared<cloud::Infrastructure>(instance.getNodes());
    cloud::Cloud c{std::make_unique<cloud::loadbalancer::LoadBalancerImpl>(
        std::make_unique<cloud::loadbalancer::strategy::RoundRobin>(infrastructure), infrastructure)};

    std::uint32_t timeSpent{0};
    while (!c.isIdle() || instance.areAnyTasksLeft())
    {
        const auto tasksInTimePoint = instance.getTasksInTimePoint(timeSpent++);
        if (!tasksInTimePoint.empty())
            c.insertTasks(tasksInTimePoint);

        c.tick();
        logger.log("tick %u", timeSpent);
    }

    logger.log("Done. Time spent: %u", timeSpent);
}

} // namespace experiment
