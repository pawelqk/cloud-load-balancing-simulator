#include "Experiment.hpp"

#include <cstdint>
#include <iostream>

#include "Cloud/CloudBuilder.hpp"
#include "Cloud/TimingServiceImpl.hpp"

namespace experiment
{

Experiment::Experiment(const instance::Instance &instance, const cloud::Policy &policy,
                       const cloud::Assessment &assessment, const logger::LoggerPtr &logger)
    : instance(instance), policy(policy), assessment(assessment), logger(logger)
{
}

void Experiment::run()
{
    cloud::CloudBuilder builder{policy, assessment};
    const auto timingService = std::make_shared<cloud::TimingServiceImpl>(logger);
    const auto cloud = builder.build(instance.getNodesMips(), timingService, logger);

    logger->info("Beginning experiment");

    while (true)
    {
        const auto currentTime = timingService->getTicks();

        cloud->tick(instance.getTasksInTimePoint(currentTime));
        if (cloud->isIdle() && instance.allTasksInserted(currentTime))
            break;

        timingService->tick();
    }

    logger->info("Done. Makespan: %u, Flowtime: %u", timingService->getTicks(), timingService->getTotalFlowtime());
}

} // namespace experiment
