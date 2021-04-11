#include <gtest/gtest.h>

#include "Cloud/Cloud.hpp"
#include "mocks/InfrastructureMock.hpp"
#include "mocks/LoadBalancerMock.hpp"

namespace cloud
{
namespace tests
{

using testing::Return;

struct CloudShould : testing::Test
{
    const mocks::InfrastructureMockPtr infrastructureMock{std::make_shared<mocks::InfrastructureMock>()};
    loadbalancer::mocks::LoadBalancerMock *loadBalancerMock{new loadbalancer::mocks::LoadBalancerMock};

    Cloud sut{loadbalancer::mocks::LoadBalancerMockPtr{loadBalancerMock}, infrastructureMock};
};

TEST_F(CloudShould, NotBeIdleWhenInfrastructureIsNotIdleOrThereAreTasksWaiting)
{
    EXPECT_CALL(*infrastructureMock, isIdle()).WillOnce(Return(false));
    EXPECT_CALL(*loadBalancerMock, areAnyTasksWaiting()).Times(0);
    ASSERT_FALSE(sut.isIdle());

    EXPECT_CALL(*infrastructureMock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*loadBalancerMock, areAnyTasksWaiting()).WillOnce(Return(true));
    ASSERT_FALSE(sut.isIdle());
}

TEST_F(CloudShould, BeIdleWhenInfrastructureIsIdleAndThereAreNoTasksWaiting)
{
    EXPECT_CALL(*infrastructureMock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*loadBalancerMock, areAnyTasksWaiting()).WillOnce(Return(false));
    ASSERT_TRUE(sut.isIdle());
}

TEST_F(CloudShould, InsertNewTasks)
{
    const TaskSet tasks{Task{0, 0, 0}, Task{3, 0, 0}};

    EXPECT_CALL(*infrastructureMock, advanceProcessing()).WillOnce(Return(TaskSet{}));
    EXPECT_CALL(*loadBalancerMock, scheduleNewTasks(tasks));
    sut.tick({tasks.cbegin(), tasks.cend()});
}

TEST_F(CloudShould, NotScheduleWaitingTasksIfThereAreNotAnyFinished)
{
    EXPECT_CALL(*infrastructureMock, advanceProcessing()).WillOnce(Return(TaskSet{}));
    EXPECT_CALL(*loadBalancerMock, scheduleWaitingTasks()).Times(0);
    sut.tick({});
}

TEST_F(CloudShould, NotScheduleWaitingTasksIfThereAreNotAnyWaiting)
{
    const TaskSet tasks{Task{1, 2, 3}, Task{4, 5, 6}};

    EXPECT_CALL(*infrastructureMock, advanceProcessing()).WillOnce(Return(tasks));
    EXPECT_CALL(*loadBalancerMock, areAnyTasksWaiting()).WillOnce(Return(false));
    EXPECT_CALL(*loadBalancerMock, scheduleWaitingTasks()).Times(0);
    sut.tick({});
}

TEST_F(CloudShould, ScheduleWaitingTasksIfThereAreWaiting)
{
    const TaskSet tasks{Task{1, 2, 3}, Task{4, 5, 6}};

    EXPECT_CALL(*infrastructureMock, advanceProcessing()).WillOnce(Return(tasks));
    EXPECT_CALL(*loadBalancerMock, areAnyTasksWaiting()).WillOnce(Return(true));
    EXPECT_CALL(*loadBalancerMock, scheduleWaitingTasks());
    sut.tick({});
}

} // namespace tests
} // namespace cloud
