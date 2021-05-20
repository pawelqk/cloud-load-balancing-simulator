#include "ExperimentRunner.hpp"

#include <future>
#include <string>
#include <vector>

#include "Logger/Files.hpp"
#include "Logger/Stdout.hpp"

namespace experiment
{

ExperimentRunner::ExperimentRunner(const std::vector<configuration::Instance> &instances, const Config &config,
                                   logger::ResultWriterPtr &&resultWriter)
    : instances(instances), config(config), resultWriter(std::move(resultWriter))
{
}

void ExperimentRunner::run(const cloud::loadbalancer::policy::PolicyBuilderPtr &policyBuilder,
                           const configuration::GeneralConfiguration &configuration)
{
    logger::Logger logger{"ExperimentRunner", config.debug};
    if (config.stdout)
        logger.addLoggingEndpoint(std::make_unique<logger::Stdout>());

    const auto description = policyBuilder->toString();
    logger.info("Running experiments for %s", description.c_str());

    std::vector<std::future<Experiment::Result>> futures;
    for (auto &&seed : configuration.seeds)
    {
        logger.info("Seed: %u", seed);
        for (auto &&penaltyFactor : configuration.penaltyFactors)
        {
            logger.info("Penalty factor: %f", penaltyFactor);
            for (auto &&instance : instances)
            {
                const auto builder = policyBuilder->clone();
                builder->setInstance(instance);

                const std::string runName{"Experiment-" + builder->toString() + "-" + std::to_string(instance.getId())};
                const auto logger = std::make_shared<logger::Logger>(runName, config.debug);
                if (config.stdout)
                    logger->addLoggingEndpoint(std::make_unique<logger::Stdout>());
                if (config.files)
                    logger->addLoggingEndpoint(std::make_unique<logger::Files>("logs/" + description, runName));

                futures.emplace_back(std::async([instance, builder, penaltyFactor, logger, seed]() {
                    Experiment e{instance, builder, penaltyFactor, logger, seed};
                    return e.run();
                }));
            }
        }
    }

    std::vector<Experiment::Result> results(futures.size());
    for (auto i = 0u; i < futures.size(); ++i)
    {
        results[i] = futures[i].get();
        logger.info(std::string{"Experiment " + std::to_string(i) + " finished"}.c_str());
    }

    resultWriter->writeResults(description, results);
}

} // namespace experiment
