#include "Experiment.hpp"

#include <cstdint>
#include <iostream>

#include "../Cloud.hpp"
#include "../LoadBalancer/LoadBalancerImpl.hpp"

namespace experiment
{

Experiment::Experiment(const instance::Instance &instance) : instance(instance)
{
}

void Experiment::run()
{
    Cloud c{instance.getNodes(), std::make_unique<loadbalancer::LoadBalancerImpl>()};

    c.insertTasks(instance.getTasks());

    std::uint32_t timeSpent{0};
    while (!c.isIdle())
    {
        c.tick();
        if (c.isIdle())
            break;

        ++timeSpent;
    }

    std::cout << "Done. time spent: " << timeSpent << std::endl;
}

} // namespace experiment
