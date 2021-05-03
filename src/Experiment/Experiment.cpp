#include "Experiment.hpp"

#include <cstdint>
#include <iostream>

#include "Cloud/CloudBuilder.hpp"
#include "Cloud/TimingServiceImpl.hpp"
#include "Utility/RandomNumberGenerator.hpp"

namespace experiment
{

Experiment::Experiment(const configuration::Instance &instance,
                       const cloud::loadbalancer::policy::PolicyBuilderPtr &policyBuilder,
                       const logger::LoggerPtr &logger)
    : instance(instance), policyBuilder(policyBuilder), logger(logger),
      timingService(std::make_shared<cloud::TimingServiceImpl>(logger))
{
    policyBuilder->setTimingService(timingService);
    cloud = cloud::CloudBuilder{instance.getNodesMips(), timingService, policyBuilder, logger}.build();
}

Experiment::Result Experiment::run(const std::uint_fast64_t seed)
{
    logger->info("Beginning experiment with seed: %u", seed);
    utility::RandomNumberGenerator::getInstance(seed);
    logger->info("Instance: \n%s", instance.toString().c_str());

    while (true)
    {
        const auto currentTime = timingService->getTicks();

        cloud->tick(instance.getTasksInTimePoint(currentTime));
        if (cloud->isIdle() && instance.allTasksInserted(currentTime))
            break;

        timingService->tick();
    }

    const auto makespan = timingService->getTicks();
    const auto flowtime = timingService->getTotalFlowtime();

    logger->info("Done. Makespan: %u, Flowtime: %u", makespan, flowtime);

    return {instance.getId(), makespan, flowtime};
}

} // namespace experiment
