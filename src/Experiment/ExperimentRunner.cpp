#include "ExperimentRunner.hpp"

#include <future>
#include <string>
#include <vector>

#include "Experiment.hpp"
#include "Logger/Files.hpp"
#include "Logger/Stdout.hpp"
#include "Utility/RandomNumberGenerator.hpp"

namespace experiment
{

ExperimentRunner::ExperimentRunner(const std::vector<instance::Instance> &instances, const Config &config)
    : instances(instances), config(config)
{
}

void ExperimentRunner::run(const cloud::Policy &policy, const cloud::Assessment &assessment,
                           const std::uint_fast64_t seed)
{
    logger::Logger logger{"ExperimentRunner", config.debug};
    if (config.stdout)
        logger.addLoggingEndpoint(std::make_unique<logger::Stdout>());

    logger.info("Running experiments");
    logger.info("Seed: %u", seed);

    std::vector<std::future<void>> futures;
    futures.reserve(instances.size());
    for (auto i = 0u; i < instances.size(); ++i)
    {
        const std::string runName{"Experiment-" + std::to_string(i)};
        const auto logger = std::make_shared<logger::Logger>(runName, config.debug);
        if (config.stdout)
            logger->addLoggingEndpoint(std::make_unique<logger::Stdout>());
        if (config.files)
            logger->addLoggingEndpoint(
                std::make_unique<logger::Files>("logs/" + toString(policy) + "/" + toString(assessment), runName));

        futures.emplace_back(std::async([instance = instances[i], policy, assessment, logger, seed]() {
            Experiment e{instance, policy, assessment, logger};
            e.run(seed);
        }));
    }

    for (auto i = 0u; i < instances.size(); ++i)
    {
        futures[i].get();
        logger.info(std::string{"Experiment " + std::to_string(i) + " finished"}.c_str());
    }
}

} // namespace experiment
