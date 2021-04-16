#include "Experiment.hpp"

#include <cstdint>
#include <iostream>

#include "Cloud/CloudBuilder.hpp"

namespace experiment
{

Experiment::Experiment(const instance::Instance &instance, const logger::LoggerPtr &logger)
    : instance(instance), logger(logger)
{
}

void Experiment::run()
{
    cloud::CloudBuilder builder;
    const auto cloud = builder.build(instance.getNodesMips(), logger);

    // NOTE: tasks of length 0 will be immediately kicked out without even noticing them
    logger->info("Beginning experiment");
    std::uint32_t timeSpent{0};
    while (true)
    {
        cloud->tick(instance.getTasksInTimePoint(timeSpent));

        if (cloud->isIdle() && instance.allTasksInserted(timeSpent))
            break;

        logger->debug("tick %u", ++timeSpent);
    }

    logger->info("Done. Time spent: %u", timeSpent);
}

} // namespace experiment
