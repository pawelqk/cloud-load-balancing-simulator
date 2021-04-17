#include "MappingAssessorBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{

MappingAssessorBase::MappingAssessorBase(const DifferenceCalculatorPtr &differenceCalculator)
    : differenceCalculator(differenceCalculator)
{
}

} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
