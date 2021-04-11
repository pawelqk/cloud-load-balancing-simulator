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
    MOCK_METHOD(MappingActions, buildTaskToNodeMapping, (const TaskSet &), (override));
};

using PolicyMockPtr = std::unique_ptr<PolicyMock>;

} // namespace mocks
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
