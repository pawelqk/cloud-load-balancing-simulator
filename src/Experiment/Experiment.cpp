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

    // NOTE: currently there is no difference between tasks having 0 or 1 length (as there's no difference between these
    // values) when using tick() method. To make it work there would have to be a separate initializing method that
    // would kick out all 0-length tasks
    std::uint32_t timeSpent{0};
    while (true)
    {
        cloud->tick();

        const auto tasksInTimePoint = instance.getTasksInTimePoint(timeSpent);
        if (!tasksInTimePoint.empty())
            cloud->insertTasks(tasksInTimePoint);

        if (cloud->isIdle() && instance.allTasksInserted(timeSpent))
            break;

        logger.log("tick %u", ++timeSpent);
    }

    logger.log("Done. Time spent: %u", timeSpent);
}

} // namespace experiment
