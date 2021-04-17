#pragma once

#include "DifferenceCalculator.hpp"
#include "MappingAssessor.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{

class MappingAssessorBase : public MappingAssessor
{
  protected:
    MappingAssessorBase(const DifferenceCalculatorPtr &differenceCalculator);

    const DifferenceCalculatorPtr differenceCalculator;
};

} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
