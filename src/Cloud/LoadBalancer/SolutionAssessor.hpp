#pragma once

#include <memory>

#include "Policy/Policy.hpp"

namespace cloud
{
namespace loadbalancer
{

class SolutionAssessor
{
  public:
    virtual double assess(const policy::Solution &solution) = 0;
};

using SolutionAssessorPtr = std::unique_ptr<SolutionAssessor>;

} // namespace loadbalancer
} // namespace cloud
