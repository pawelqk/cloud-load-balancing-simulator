#include "Experiment.hpp"

#include <cstdint>
#include <iostream>

#include "../Cloud/Cloud.hpp"
#include "../Cloud/LoadBalancer/LoadBalancerImpl.hpp"

namespace experiment
{

Experiment::Experiment(const instance::Instance &instance) : instance(instance)
{
}

void Experiment::run()
{
    cloud::Cloud c{std::make_unique<cloud::loadbalancer::LoadBalancerImpl>(instance.getNodes())};

    c.insertTasks(instance.getTasks());

    std::uint32_t timeSpent{0};
    while (!c.isIdle())
    {
        ++timeSpent;
        c.tick();
    }

    std::cout << "Done. time spent: " << timeSpent << std::endl;
}

} // namespace experiment
