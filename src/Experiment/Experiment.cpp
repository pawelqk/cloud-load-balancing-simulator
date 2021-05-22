#include "Experiment.hpp"

#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "Cloud/CloudBuilder.hpp"
#include "Cloud/TimingServiceImpl.hpp"
#include "Utility/RandomNumberGenerator.hpp"

namespace experiment
{

Experiment::Experiment(const configuration::Instance &instance,
                       const cloud::loadbalancer::policy::PolicyBuilderPtr &policyBuilder, const double penaltyFactor,
                       const logger::LoggerPtr &logger, const std::uint_fast64_t seed)
    : instance(instance), policyBuilder(policyBuilder), logger(logger), penaltyFactor(penaltyFactor),
      timingService(std::make_shared<cloud::TimingServiceImpl>(logger)), seed(seed)
{
    policyBuilder->setPenaltyFactor(penaltyFactor);
    policyBuilder->setTimingService(timingService);
    cloud =
        cloud::CloudBuilder{instance.getNodesData(), timingService, policyBuilder, seed, penaltyFactor, logger}.build();
}

Experiment::Result Experiment::run()
{
    logger->debug("Instance: \n%s", instance.toString().c_str());

    while (true)
    {
        const auto currentTime = timingService->getTicks();

        cloud->tick(instance.getTasksInTimePoint(currentTime));
        if (cloud->isIdle() && instance.allTasksInserted(currentTime))
            break;

        timingService->tick();
    }

    if (!cloud->areAllInsertedTasksFinished())
        throw std::runtime_error("Some tasks were lost during experiment execution");

    const auto makespan = timingService->getTicks();
    const auto flowtime = timingService->getTotalFlowtime();

    logger->info("Done. Makespan: %u, Flowtime: %u", makespan, flowtime);

    return {instance.getId(), makespan, flowtime};
}

} // namespace experiment
