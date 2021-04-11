#pragma once

#include "Cloud/LoadBalancer/Policy/Policy.hpp"
#include "SolutionAssessorBase.hpp"

namespace cloud
{
namespace loadbalancer
{

class MakespanAssessor : public SolutionAssessorBase
{
  public:
    MakespanAssessor(const InfrastructureCPtr &infrastructure);

    double assess(const policy::Solution &solution) override;
};

} // namespace loadbalancer
} // namespace cloud
