#pragma once

#include "Cloud/LoadBalancer/Policy/Policy.hpp"
#include "SolutionAssessorBase.hpp"

namespace cloud
{
namespace loadbalancer
{

class FlowtimeAssessor : public SolutionAssessorBase
{
  public:
    FlowtimeAssessor(const InfrastructureCPtr &infrastructure);

    double assess(const policy::Solution &solution) override;
};

} // namespace loadbalancer
} // namespace cloud
