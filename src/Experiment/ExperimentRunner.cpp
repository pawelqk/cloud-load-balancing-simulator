#include "ExperimentRunner.hpp"

#include <future>
#include <string>
#include <vector>

#include "Experiment.hpp"
#include "Logger/Files.hpp"
#include "Logger/Stdout.hpp"

namespace experiment
{

ExperimentRunner::ExperimentRunner(const std::vector<instance::Instance> &instances) : instances(instances)
{
}

void ExperimentRunner::run(bool withDebug, bool withStdout)
{
    logger::Logger logger{"ExperimentRunner", false};
    logger.addLoggingEndpoint(std::make_unique<logger::Stdout>());

    logger.info("Running experiments");
    std::vector<std::future<void>> futures;
    futures.reserve(instances.size());
    for (auto i = 0u; i < instances.size(); ++i)
    {
        const auto logger = std::make_shared<logger::Logger>("Experiment" + std::to_string(i), withDebug);
        if (withStdout)
            logger->addLoggingEndpoint(std::make_unique<logger::Stdout>());
        logger->addLoggingEndpoint(std::make_unique<logger::Files>("log", "Experiment" + std::to_string(i)));
        futures.emplace_back(std::async([logger, instance = instances[i]]() {
            Experiment e{instance, logger};
            e.run();
        }));
    }

    for (auto i = 0u; i < instances.size(); ++i)
    {
        futures[i].get();
        logger.info(std::string{"Task " + std::to_string(i) + " finished"}.c_str());
    }
}

} // namespace experiment
