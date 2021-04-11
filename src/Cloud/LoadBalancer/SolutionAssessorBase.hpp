#pragma once

#include "SolutionAssessor.hpp"

namespace cloud
{
namespace loadbalancer
{

class SolutionAssessorBase : public SolutionAssessor
{
  protected:
    SolutionAssessorBase(const InfrastructureCPtr &infrastructure);

    const InfrastructureCPtr infrastructure;
};

} // namespace loadbalancer
} // namespace cloud
