#pragma once

#include <memory>

#include <gmock/gmock.h>

#include "Cloud/LoadBalancer/Mapping/DifferenceCalculator.hpp"
#include "Cloud/LoadBalancer/Policy/Policy.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{
namespace mocks
{

class DifferenceCalculatorMock : public testing::StrictMock<DifferenceCalculator>
{
  public:
    MOCK_METHOD(Difference, calculate, (const policy::NodeToTaskMapping &), (const, override));
};

using DifferenceCalculatorMockPtr = std::shared_ptr<DifferenceCalculatorMock>;

} // namespace mocks
} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
