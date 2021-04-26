#pragma once

#include "Cloud/CloudBuilder.hpp"
#include "Instance/Instance.hpp"

namespace experiment
{

class Experiment
{
  public:
    Experiment(const instance::Instance &instance, const cloud::Policy &policy, const cloud::Assessment &assessment,
               const logger::LoggerPtr &logger);

    void run(const std::uint_fast64_t seed);

  private:
    instance::Instance instance;
    const cloud::Policy policy;
    const cloud::Assessment assessment;
    const logger::LoggerPtr logger;
};

} // namespace experiment
