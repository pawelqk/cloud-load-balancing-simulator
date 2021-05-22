#pragma once

#include "Cloud/CloudBuilder.hpp"
#include "Cloud/TimingService.hpp"
#include "Configuration/Instance.hpp"
#include <cstdint>

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
        std::uint_fast64_t seed;
        double penaltyFactor;
    };

    Experiment(const configuration::Instance &instance,
               const cloud::loadbalancer::policy::PolicyBuilderPtr &policyBuilder, const double penaltyFactor,
               const logger::LoggerPtr &logger, const std::uint_fast64_t seed);

    Result run();

    std::string toString();

  private:
    configuration::Instance instance;
    cloud::loadbalancer::policy::PolicyBuilderPtr policyBuilder;
    const double penaltyFactor;
    logger::LoggerPtr logger;
    cloud::TimingServicePtr timingService;
    std::unique_ptr<cloud::Cloud> cloud;
    const std::uint_fast64_t seed;
};

} // namespace experiment
