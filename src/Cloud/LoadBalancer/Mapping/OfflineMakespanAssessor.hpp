#pragma once

#include "Cloud/LoadBalancer/Policy/Policy.hpp"
#include "MappingAssessorBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{

class OfflineMakespanAssessor : public MappingAssessor
{
  public:
    double assess(const policy::NodeToTaskMapping &mapping) override;
};

} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
