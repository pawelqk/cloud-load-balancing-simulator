#pragma once

#include <cstdint>
#include <vector>

#include "Cloud/CloudBuilder.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBuilder.hpp"
#include "Configuration/ConfigurationReader.hpp"
#include "Configuration/Instance.hpp"
#include "Experiment.hpp"
#include "Logger/ResultWriter.hpp"

namespace experiment
{

class ExperimentRunner
{
  public:
    struct Config
    {
        bool debug;
        bool stdout;
        bool files;
    };

    ExperimentRunner(const std::vector<configuration::Instance> &instances, const Config &config,
                     logger::ResultWriterPtr &&resultWriter);

    void run(const cloud::loadbalancer::policy::PolicyBuilderPtr &policyBuilder,
             const configuration::GeneralConfiguration &configuration);

  private:
    void writeResultsToFile(const std::vector<Experiment::Result> &results);

    const std::vector<configuration::Instance> instances;
    const Config config;
    const logger::ResultWriterPtr resultWriter;
};

} // namespace experiment
