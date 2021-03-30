#include "Experiment.hpp"

#include <cstdint>
#include <iostream>

#include "Cloud/Cloud.hpp"
#include "Cloud/LoadBalancer/LoadBalancerImpl.hpp"
#include "Cloud/LoadBalancer/Strategy/RoundRobin.hpp"

namespace experiment
{

Experiment::Experiment(const instance::Instance &instance) : instance(instance), logger("Experiment")
{
}

void Experiment::run()
{
    cloud::Cloud c{std::make_unique<cloud::loadbalancer::LoadBalancerImpl>(
        std::make_unique<cloud::loadbalancer::strategy::RoundRobin>(), instance.getNodes())};

    c.insertTasks(instance.getTasks());

    std::uint32_t timeSpent{0};
    while (!c.isIdle())
    {
        ++timeSpent;
        logger.log("tick %u", timeSpent);
        c.tick();
    }

    logger.log("Done. Time spent: %u", timeSpent);
}

} // namespace experiment
