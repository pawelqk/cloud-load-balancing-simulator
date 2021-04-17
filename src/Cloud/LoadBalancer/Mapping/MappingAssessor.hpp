#pragma once

#include <memory>

#include "Cloud/LoadBalancer/Policy/Policy.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{

class MappingAssessor
{
  public:
    virtual double assess(const policy::NodeToTaskMapping &mapping) = 0;
};

using MappingAssessorPtr = std::unique_ptr<MappingAssessor>;

} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
