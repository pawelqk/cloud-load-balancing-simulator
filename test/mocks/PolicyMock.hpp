#pragma once

#include <memory>

#include <gmock/gmock.h>

#include "Cloud/LoadBalancer/Policy/Policy.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace mocks
{

class PolicyMock : public testing::StrictMock<Policy>
{
  public:
    MOCK_METHOD(NodeToTaskMapping, buildNodeToTaskMapping, (const TaskPtrVec &), (override));
};

using PolicyMockPtr = std::unique_ptr<PolicyMock>;

} // namespace mocks
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
