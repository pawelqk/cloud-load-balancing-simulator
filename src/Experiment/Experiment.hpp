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

    void run();

  private:
    instance::Instance instance;
    const cloud::Policy policy;
    const cloud::Assessment assessment;
    const logger::LoggerPtr logger;
};

} // namespace experiment
