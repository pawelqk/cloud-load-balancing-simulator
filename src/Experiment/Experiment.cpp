#include "Experiment.hpp"

#include <cstdint>
#include <iostream>

#include "Cloud/Cloud.hpp"
#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/LoadBalancerImpl.hpp"
#include "Cloud/LoadBalancer/Strategy/Random.hpp"
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

    // TODO: currently there is no difference between tasks having 0 or 1 length (as there's no difference between these
    // values) when using tick() method. To make it work there would have to be a separate initializing method that
    // would kick out all 0-length tasks
    std::uint32_t timeSpent{0};
    while (true)
    {
        c.tick();

        const auto tasksInTimePoint = instance.getTasksInTimePoint(timeSpent);
        if (!tasksInTimePoint.empty())
            c.insertTasks(tasksInTimePoint);

        if (c.isIdle() && !instance.areAnyTasksLeft())
            break;

        logger.log("tick %u", ++timeSpent);
    }

    logger.log("Done. Time spent: %u", timeSpent);
}

} // namespace experiment
