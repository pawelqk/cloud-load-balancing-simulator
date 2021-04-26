#include <tuple>

#include <gtest/gtest.h>

#include "Cloud/Cloud.hpp"
#include "Cloud/Task.hpp"
#include "Cloud/TaskImpl.hpp"
#include "Instance/Instance.hpp"
#include "Logger/Logger.hpp"
#include "mocks/InfrastructureMock.hpp"
#include "mocks/LoadBalancerMock.hpp"
#include "mocks/TimingServiceMock.hpp"

namespace cloud
{
namespace tests
{

MATCHER_P(TasksAre, tasks, "")
{
    return std::equal(tasks.cbegin(), tasks.cend(), arg.cbegin(), [](auto &&leftTask, auto &&rightTask) {
        return leftTask->getMips() == rightTask->getMips() &&
               rightTask->getInitialLength() == rightTask->getInitialLength();
    });
}

using testing::Return;

struct CloudShould : testing::Test
{
    loadbalancer::mocks::LoadBalancerMock *loadBalancerMock{new loadbalancer::mocks::LoadBalancerMock};
    const mocks::InfrastructureMockPtr infrastructureMock{std::make_shared<mocks::InfrastructureMock>()};
    const mocks::TimingServiceMockPtr timingServiceMock{std::make_shared<mocks::TimingServiceMock>()};

    Cloud sut{loadbalancer::mocks::LoadBalancerMockPtr{loadBalancerMock}, infrastructureMock, timingServiceMock,
              std::make_shared<logger::Logger>("", false)};
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
    constexpr std::uint32_t arrivalTime{5};
    const TaskPtrVec tasks{std::make_shared<TaskImpl>(0, 2, 3, arrivalTime),
                           std::make_shared<TaskImpl>(1, 4, 6, arrivalTime)};
    const instance::TaskDataVec taskDatas{{2, 3}, {4, 6}};

    EXPECT_CALL(*infrastructureMock, advanceProcessing()).WillOnce(Return(TaskPtrVec{}));
    EXPECT_CALL(*timingServiceMock, getTicks()).WillRepeatedly(Return(arrivalTime));
    EXPECT_CALL(*loadBalancerMock, scheduleNewTasks(TasksAre(tasks)));
    sut.tick(taskDatas);
}

TEST_F(CloudShould, NotScheduleWaitingTasksIfThereAreNotAnyFinished)
{
    EXPECT_CALL(*infrastructureMock, advanceProcessing()).WillOnce(Return(TaskPtrVec{}));
    EXPECT_CALL(*timingServiceMock, getTicks()).Times(0);
    EXPECT_CALL(*loadBalancerMock, scheduleWaitingTasks()).Times(0);
    sut.tick({});
}

TEST_F(CloudShould, NotScheduleWaitingTasksIfThereAreNotAnyWaiting)
{
    const TaskPtrVec tasks{std::make_shared<TaskImpl>(1, 2, 3, 4), std::make_shared<TaskImpl>(4, 5, 6, 5)};
    EXPECT_CALL(*timingServiceMock, getTicks()).WillOnce(Return(10));

    EXPECT_CALL(*infrastructureMock, advanceProcessing()).WillOnce(Return(tasks));
    EXPECT_CALL(*timingServiceMock, addFlowtime(6 + 5));
    EXPECT_CALL(*loadBalancerMock, areAnyTasksWaiting()).WillOnce(Return(false));
    EXPECT_CALL(*loadBalancerMock, scheduleWaitingTasks()).Times(0);
    sut.tick({});
}

TEST_F(CloudShould, ScheduleWaitingTasksIfThereAreWaiting)
{
    const TaskPtrVec tasks{std::make_shared<TaskImpl>(1, 2, 3, 1), std::make_shared<TaskImpl>(4, 5, 6, 1)};
    EXPECT_CALL(*timingServiceMock, getTicks()).WillOnce(Return(2));

    EXPECT_CALL(*infrastructureMock, advanceProcessing()).WillOnce(Return(tasks));
    EXPECT_CALL(*timingServiceMock, addFlowtime(1 + 1));
    EXPECT_CALL(*loadBalancerMock, areAnyTasksWaiting()).WillOnce(Return(true));
    EXPECT_CALL(*loadBalancerMock, scheduleWaitingTasks());
    sut.tick({});
}

} // namespace tests
} // namespace cloud
