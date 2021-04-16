#include <gtest/gtest.h>

#include "Cloud/LoadBalancer/LoadBalancerImpl.hpp"
#include "mocks/InfrastructureMock.hpp"
#include "mocks/PolicyMock.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace tests
{

using testing::Return;

struct LoadBalancerShould : testing::Test
{
    const mocks::InfrastructureMockPtr infrastructureMock{std::make_shared<mocks::InfrastructureMock>()};
    policy::mocks::PolicyMock *policyMock{new policy::mocks::PolicyMock};

    LoadBalancerImpl sut{policy::mocks::PolicyMockPtr{policyMock}, infrastructureMock, nullptr};
};

TEST_F(LoadBalancerShould, HaveNoTasksWaitingWhenIdle)
{
    ASSERT_FALSE(sut.areAnyTasksWaiting());
}

} // namespace tests
} // namespace loadbalancer
} // namespace cloud
