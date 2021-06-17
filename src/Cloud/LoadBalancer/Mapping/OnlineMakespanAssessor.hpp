#pragma once

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Policy/Policy.hpp"
#include "MappingAssessor.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{

class OnlineMakespanAssessor : public MappingAssessor
{
  public:
    OnlineMakespanAssessor(const InfrastructureCPtr &infrastructure);

    double assess(const policy::NodeToTaskMapping &mapping) override;

  private:
    const InfrastructureCPtr infrastructure;
};

} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
