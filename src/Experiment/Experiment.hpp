#pragma once

#include "Cloud/CloudBuilder.hpp"
#include "Cloud/TimingService.hpp"
#include "Configuration/Instance.hpp"

namespace experiment
{

class Experiment
{
  public:
    struct Result
    {
        std::uint32_t instanceId;
        std::uint32_t makespan;
        std::uint32_t flowtime;
    };

    Experiment(const configuration::Instance &instance,
               const cloud::loadbalancer::policy::PolicyBuilderPtr &policyBuilder,
               const logger::LoggerPtr &logger);

    Result run(const std::uint_fast64_t seed);

    std::string toString();

  private:
    configuration::Instance instance;
    const cloud::loadbalancer::policy::PolicyBuilderPtr policyBuilder;
    const logger::LoggerPtr logger;
    const cloud::TimingServicePtr timingService;
    std::unique_ptr<cloud::Cloud> cloud;
};

} // namespace experiment
