#pragma once

#include <vector>

#include "Cloud/CloudBuilder.hpp"
#include "Instance/Instance.hpp"

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
    ExperimentRunner(const std::vector<instance::Instance> &instances, const Config &config);

    void run(const cloud::Policy &policy, const cloud::Assessment &assessment);

  private:
    const std::vector<instance::Instance> instances;
    const Config config;
};

} // namespace experiment
