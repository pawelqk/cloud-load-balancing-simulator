#pragma once

#include <gmock/gmock.h>

#include "Cloud/LoadBalancer/LoadBalancer.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace mocks
{

class LoadBalancerMock : public testing::StrictMock<LoadBalancer>
{
  public:
    MOCK_METHOD(void, scheduleNewTasks, (const TaskSet &), (override));
    MOCK_METHOD(void, scheduleWaitingTasks, (), (override));
    MOCK_METHOD(bool, areAnyTasksWaiting, (), (const, override));
};

using LoadBalancerMockPtr = std::unique_ptr<LoadBalancerMock>;

} // namespace mocks
} // namespace loadbalancer
} // namespace cloud
