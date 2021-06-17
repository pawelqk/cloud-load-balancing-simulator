#pragma once

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Policy/Policy.hpp"
#include "Cloud/TimingService.hpp"
#include "MappingAssessor.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{

class OnlineFlowtimeAssessor : public MappingAssessor
{
  public:
    OnlineFlowtimeAssessor(const InfrastructureCPtr &infrastructure, const TimingServicePtr &timingService);

    double assess(const policy::NodeToTaskMapping &mapping) override;

  private:
    std::uint32_t calculateTaskFlowtime(const TaskPtr &task);

    const InfrastructureCPtr infrastructure;
    const TimingServicePtr timingService;
};

} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
