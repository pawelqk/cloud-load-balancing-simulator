#pragma once

#include "Cloud/LoadBalancer/Policy/Policy.hpp"
#include "Cloud/TimingService.hpp"
#include "MappingAssessorBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{

class OnlineFlowtimeAssessorWithMigrationsAndPreemptions : public MappingAssessorBase
{
  public:
    OnlineFlowtimeAssessorWithMigrationsAndPreemptions(const DifferenceCalculatorPtr &differenceCalculator,
                                                       const TimingServicePtr &timingService);

    double assess(const policy::NodeToTaskMapping &mapping) override;

  private:
    std::uint32_t calculateTaskFlowtime(const TaskPtr &task);

    const TimingServicePtr timingService;
};

} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
