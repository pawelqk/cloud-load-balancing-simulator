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

class OfflineFlowtimeAssessor : public MappingAssessor
{
  public:
    double assess(const policy::NodeToTaskMapping &mapping) override;
};

} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
