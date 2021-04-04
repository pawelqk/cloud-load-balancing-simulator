#include "Experiment.hpp"

#include <cstdint>
#include <iostream>

#include "Cloud/CloudBuilder.hpp"

namespace experiment
{

Experiment::Experiment(const instance::Instance &instance) : instance(instance), logger("Experiment")
{
}

void Experiment::run()
{
    cloud::CloudBuilder builder;
    const auto cloud = builder.build(instance.getNodesMips());

    // NOTE: tasks of length 0 will be immediately kicked out without even noticing them
    std::uint32_t timeSpent{0};
    while (true)
    {
        cloud->tick(instance.getTasksInTimePoint(timeSpent));

        if (cloud->isIdle() && instance.allTasksInserted(timeSpent))
            break;

        logger.log("tick %u", ++timeSpent);
    }

    logger.log("Done. Time spent: %u", timeSpent);
}

} // namespace experiment
