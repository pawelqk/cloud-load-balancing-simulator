#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Cloud.hpp"
#include "Infrastructure.hpp"
#include "LoadBalancer/Mapping/DifferenceCalculator.hpp"
#include "LoadBalancer/Mapping/DifferenceCalculatorImpl.hpp"
#include "LoadBalancer/Mapping/MappingAssessor.hpp"
#include "LoadBalancer/Policy/PolicyBuilder.hpp"
#include "Logger/Logger.hpp"
#include "TimingService.hpp"

namespace cloud
{

class CloudBuilder
{
  public:
    CloudBuilder(const std::vector<std::uint32_t> &nodesMips, const TimingServicePtr &timingService,
                 const cloud::loadbalancer::policy::PolicyBuilderPtr &policyBuilder, const logger::LoggerPtr &logger);

    std::unique_ptr<Cloud> build();

  private:
    const std::vector<std::uint32_t> nodesMips;
    const TimingServicePtr timingService;
    const cloud::loadbalancer::policy::PolicyBuilderPtr policyBuilder;
    const logger::LoggerPtr logger;
};

using CloudBuilderPtr = std::shared_ptr<CloudBuilder>;

} // namespace cloud
