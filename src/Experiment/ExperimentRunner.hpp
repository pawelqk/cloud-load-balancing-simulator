#pragma once

#include <cstdint>
#include <vector>

#include "Cloud/CloudBuilder.hpp"
#include "Cloud/LoadBalancer/Policy/Builders/PolicyBuilder.hpp"
#include "Experiment.hpp"
#include "Instance/Instance.hpp"
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

    ExperimentRunner(const std::vector<instance::Instance> &instances, const Config &config,
                     logger::ResultWriterPtr &&resultWriter);

    void run(const cloud::loadbalancer::policy::builders::PolicyBuilderPtr &policyBuilder,
             const std::uint_fast64_t seed);

  private:
    void writeResultsToFile(const std::vector<Experiment::Result> &results);

    const std::vector<instance::Instance> instances;
    const Config config;
    const logger::ResultWriterPtr resultWriter;
};

} // namespace experiment
