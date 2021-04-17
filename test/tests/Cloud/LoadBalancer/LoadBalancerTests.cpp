#include "gmock/gmock-spec-builders.h"
#include <gtest/gtest.h>

#include "Cloud/LoadBalancer/LoadBalancerImpl.hpp"
#include "Cloud/LoadBalancer/Mapping/DifferenceCalculator.hpp"
#include "Logger/Logger.hpp"
#include "mocks/DifferenceCalculatorMock.hpp"
#include "mocks/InfrastructureMock.hpp"
#include "mocks/NodeMock.hpp"
#include "mocks/PolicyMock.hpp"
#include "mocks/TaskMock.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace tests
{

using testing::_;
using testing::Return;
using testing::ReturnRef;
using testing::Sequence;
using testing::UnorderedElementsAreArray;

struct LoadBalancerShould : testing::Test
{
    void expectGettingNodes(std::vector<NodePtr> &nodeMocks)
    {
        EXPECT_CALL(*infrastructureMock, getNodes()).WillRepeatedly(ReturnRef(nodeMocks));
    }

    policy::mocks::PolicyMock *policyMock{new policy::mocks::PolicyMock};
    const mocks::InfrastructureMockPtr infrastructureMock{std::make_shared<mocks::InfrastructureMock>()};
    const mapping::mocks::DifferenceCalculatorMockPtr differenceCalculatorMock{
        std::make_shared<mapping::mocks::DifferenceCalculatorMock>()};

    LoadBalancerImpl sut{policy::mocks::PolicyMockPtr{policyMock}, infrastructureMock, differenceCalculatorMock,
                         std::make_shared<logger::Logger>("", false)};
};

TEST_F(LoadBalancerShould, HaveNoTasksWaitingWhenIdle)
{
    ASSERT_FALSE(sut.areAnyTasksWaiting());
}

TEST_F(LoadBalancerShould, ScheduleWaitingTasksIfThereAreAny)
{
    const TaskPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const TaskPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const TaskPtr task2Mock = std::make_shared<mocks::TaskMock>(2);
    const TaskPtr task3Mock = std::make_shared<mocks::TaskMock>(3);
    const TaskPtrVec firstlyInsertedTaskMocks{task0Mock, task1Mock, task2Mock};
    const TaskPtrVec waitingTaskMocks{task2Mock};
    const TaskPtrVec secondlyInsertedTaskMocks{task3Mock};

    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    const mocks::NodeMockPtr node2Mock = std::make_shared<mocks::NodeMock>(2);
    NodePtrVec nodeMocks{node0Mock, node1Mock, node2Mock};

    const policy::NodeToTaskMapping mapping{{0, {task1Mock, task2Mock}}, {1, {task0Mock}}};
    const mapping::Difference difference{};

    EXPECT_CALL(*policyMock, buildNodeToTaskMapping(firstlyInsertedTaskMocks)).WillOnce(Return(mapping));
    EXPECT_CALL(*differenceCalculatorMock, calculate(mapping)).WillOnce(Return(difference));
    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*node0Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node1Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node2Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node0Mock, assign(task1Mock));
    EXPECT_CALL(*node1Mock, assign(task0Mock));
    EXPECT_CALL(*node2Mock, assign(_)).Times(0);
    sut.scheduleNewTasks(firstlyInsertedTaskMocks);

    ASSERT_TRUE(sut.areAnyTasksWaiting());

    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*node0Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node1Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node2Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node0Mock, assign(task2Mock));
    EXPECT_CALL(*node1Mock, assign(_)).Times(0);
    EXPECT_CALL(*node2Mock, assign(_)).Times(0);
    sut.scheduleWaitingTasks();

    ASSERT_FALSE(sut.areAnyTasksWaiting());
}

/* NOTE: currently load balancer does not pass tasks already assigned to nodes (balancing policies fetch this
 * information by themselves). This might change in the future */
TEST_F(LoadBalancerShould, PutWaitingTasksAlongsideNewOnes)
{
    const TaskPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const TaskPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const TaskPtr task2Mock = std::make_shared<mocks::TaskMock>(1);
    const TaskPtr task3Mock = std::make_shared<mocks::TaskMock>(1);
    const TaskPtrVec firstlyInsertedTaskMocks{task0Mock, task1Mock, task2Mock};
    const TaskPtrVec waitingTaskMocks{task2Mock};
    const TaskPtrVec secondlyInsertedTaskMocks{task3Mock};

    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    NodePtrVec nodeMocks{node0Mock, node1Mock};

    auto insertFirstBatchOfTasks = [&]() {
        const policy::NodeToTaskMapping mapping{{0, {task1Mock, task2Mock}}, {1, {task0Mock}}};
        const mapping::Difference difference{};

        EXPECT_CALL(*policyMock, buildNodeToTaskMapping(firstlyInsertedTaskMocks)).WillOnce(Return(mapping));
        EXPECT_CALL(*differenceCalculatorMock, calculate(mapping)).WillOnce(Return(difference));
        expectGettingNodes(nodeMocks);
        EXPECT_CALL(*node0Mock, isIdle()).WillOnce(Return(true));
        EXPECT_CALL(*node1Mock, isIdle()).WillOnce(Return(true));
        EXPECT_CALL(*node0Mock, assign(task1Mock));
        EXPECT_CALL(*node1Mock, assign(task0Mock));
        sut.scheduleNewTasks(firstlyInsertedTaskMocks);

        ASSERT_TRUE(sut.areAnyTasksWaiting());
    };

    auto insertSecondBatchOfTasks = [&]() {
        const policy::NodeToTaskMapping mapping{{0, {task1Mock, task2Mock}}, {1, {task0Mock, task3Mock}}};
        mapping::Difference difference;
        difference.nodeIdsWithoutChange.push_back(0);
        difference.nodeIdsWithoutChange.push_back(1);

        auto tasksThatWillBeMapped = secondlyInsertedTaskMocks;
        tasksThatWillBeMapped.insert(tasksThatWillBeMapped.end(), waitingTaskMocks.begin(), waitingTaskMocks.end());

        EXPECT_CALL(*policyMock, buildNodeToTaskMapping(UnorderedElementsAreArray(tasksThatWillBeMapped)))
            .WillOnce(Return(mapping));
        EXPECT_CALL(*differenceCalculatorMock, calculate(mapping)).WillOnce(Return(difference));
        expectGettingNodes(nodeMocks);
        EXPECT_CALL(*node0Mock, isIdle()).WillOnce(Return(false));
        EXPECT_CALL(*node1Mock, isIdle()).WillOnce(Return(false));
        sut.scheduleNewTasks(secondlyInsertedTaskMocks);

        ASSERT_TRUE(sut.areAnyTasksWaiting());
    };

    insertFirstBatchOfTasks();

    insertSecondBatchOfTasks();
}

TEST_F(LoadBalancerShould, HandlePreemptions)
{
    const mocks::TaskMockPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const mocks::TaskMockPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const mocks::TaskMockPtr task2Mock = std::make_shared<mocks::TaskMock>(2);
    const mocks::TaskMockPtr task3Mock = std::make_shared<mocks::TaskMock>(3);
    const TaskPtrVec firstlyInsertedTaskMocks{task0Mock, task1Mock};
    const TaskPtrVec secondlyInsertedTaskMocks{task2Mock, task3Mock};

    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    NodePtrVec nodeMocks{node0Mock, node1Mock};

    const policy::NodeToTaskMapping mapping{{0, {task0Mock}}, {1, {task1Mock}}};
    const mapping::Difference difference{};

    EXPECT_CALL(*policyMock, buildNodeToTaskMapping(firstlyInsertedTaskMocks)).WillOnce(Return(mapping));
    EXPECT_CALL(*differenceCalculatorMock, calculate(mapping)).WillOnce(Return(difference));
    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*node0Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node1Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node0Mock, assign(std::static_pointer_cast<Task>(task0Mock)));
    EXPECT_CALL(*node1Mock, assign(std::static_pointer_cast<Task>(task1Mock)));
    sut.scheduleNewTasks(firstlyInsertedTaskMocks);

    ASSERT_FALSE(sut.areAnyTasksWaiting());

    const policy::NodeToTaskMapping newMapping{{0, {task2Mock, task1Mock}}, {1, {task3Mock, task0Mock}}};
    mapping::Difference newDifference;
    newDifference.preemptions.emplace_back(0, task0Mock);
    newDifference.preemptions.emplace_back(1, task1Mock);

    EXPECT_CALL(*policyMock, buildNodeToTaskMapping(secondlyInsertedTaskMocks)).WillOnce(Return(newMapping));
    EXPECT_CALL(*differenceCalculatorMock, calculate(newMapping)).WillOnce(Return(newDifference));
    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*node0Mock, extractTask()).WillOnce(Return(task0Mock));
    EXPECT_CALL(*task0Mock, performPreemption());
    EXPECT_CALL(*node1Mock, extractTask()).WillOnce(Return(task1Mock));
    EXPECT_CALL(*task1Mock, performPreemption());
    EXPECT_CALL(*node0Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node1Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node0Mock, assign(std::static_pointer_cast<Task>(task2Mock)));
    EXPECT_CALL(*node1Mock, assign(std::static_pointer_cast<Task>(task3Mock)));
    sut.scheduleNewTasks(secondlyInsertedTaskMocks);

    ASSERT_TRUE(sut.areAnyTasksWaiting());

    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*node0Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node1Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node0Mock, assign(std::static_pointer_cast<Task>(task1Mock)));
    EXPECT_CALL(*node1Mock, assign(std::static_pointer_cast<Task>(task0Mock)));
    sut.scheduleWaitingTasks();

    ASSERT_FALSE(sut.areAnyTasksWaiting());
}

TEST_F(LoadBalancerShould, HandleMigrations)
{
    const mocks::TaskMockPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const mocks::TaskMockPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const mocks::TaskMockPtr task2Mock = std::make_shared<mocks::TaskMock>(2);
    const mocks::TaskMockPtr task3Mock = std::make_shared<mocks::TaskMock>(3);
    const TaskPtrVec firstlyInsertedTaskMocks{task0Mock, task1Mock, task2Mock};
    const TaskPtrVec secondlyInsertedTaskMocks{task3Mock};

    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    const mocks::NodeMockPtr node2Mock = std::make_shared<mocks::NodeMock>(2);
    const mocks::NodeMockPtr node3Mock = std::make_shared<mocks::NodeMock>(3);
    NodePtrVec nodeMocks{node0Mock, node1Mock, node2Mock, node3Mock};

    const policy::NodeToTaskMapping mapping{{0, {task0Mock}}, {1, {task1Mock}}, {3, {task2Mock}}};
    const mapping::Difference difference{};

    EXPECT_CALL(*policyMock, buildNodeToTaskMapping(firstlyInsertedTaskMocks)).WillOnce(Return(mapping));
    EXPECT_CALL(*differenceCalculatorMock, calculate(mapping)).WillOnce(Return(difference));
    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*node0Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node1Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node2Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node3Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node0Mock, assign(std::static_pointer_cast<Task>(task0Mock)));
    EXPECT_CALL(*node1Mock, assign(std::static_pointer_cast<Task>(task1Mock)));
    EXPECT_CALL(*node3Mock, assign(std::static_pointer_cast<Task>(task2Mock)));
    sut.scheduleNewTasks(firstlyInsertedTaskMocks);

    ASSERT_FALSE(sut.areAnyTasksWaiting());

    const policy::NodeToTaskMapping newMapping{{0, {task1Mock}}, {1, {task0Mock}}, {2, {task2Mock}}, {3, {task3Mock}}};
    mapping::Difference newDifference;
    newDifference.migrations.emplace_back(0, 1, task0Mock);
    newDifference.migrations.emplace_back(1, 0, task1Mock);
    newDifference.migrations.emplace_back(3, 2, task2Mock);

    EXPECT_CALL(*policyMock, buildNodeToTaskMapping(secondlyInsertedTaskMocks)).WillOnce(Return(newMapping));
    EXPECT_CALL(*differenceCalculatorMock, calculate(newMapping)).WillOnce(Return(newDifference));
    expectGettingNodes(nodeMocks);
    {
        Sequence checkThatNothingGetsOverwritten;
        EXPECT_CALL(*node0Mock, extractTask()).WillOnce(Return(task0Mock));
        EXPECT_CALL(*task0Mock, performMigration());
        EXPECT_CALL(*node1Mock, extractTask()).WillOnce(Return(task1Mock));
        EXPECT_CALL(*task1Mock, performMigration());
        EXPECT_CALL(*node3Mock, extractTask()).WillOnce(Return(task2Mock));
        EXPECT_CALL(*task2Mock, performMigration());
        EXPECT_CALL(*node1Mock, assign(std::static_pointer_cast<Task>(task0Mock)));
        EXPECT_CALL(*node0Mock, assign(std::static_pointer_cast<Task>(task1Mock)));
        EXPECT_CALL(*node2Mock, assign(std::static_pointer_cast<Task>(task2Mock)));

        EXPECT_CALL(*node0Mock, isIdle()).WillOnce(Return(false));
        EXPECT_CALL(*node1Mock, isIdle()).WillOnce(Return(false));
        EXPECT_CALL(*node2Mock, isIdle()).WillOnce(Return(false));
        EXPECT_CALL(*node3Mock, isIdle()).WillOnce(Return(true));
        EXPECT_CALL(*node3Mock, assign(std::static_pointer_cast<Task>(task3Mock)));
    }
    sut.scheduleNewTasks(secondlyInsertedTaskMocks);

    ASSERT_FALSE(sut.areAnyTasksWaiting());
}

TEST_F(LoadBalancerShould, HandleDiffWithEverything)
{
    const mocks::TaskMockPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const mocks::TaskMockPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const mocks::TaskMockPtr task2Mock = std::make_shared<mocks::TaskMock>(2);
    const mocks::TaskMockPtr task3Mock = std::make_shared<mocks::TaskMock>(3);
    const TaskPtrVec firstlyInsertedTaskMocks{task0Mock, task1Mock, task2Mock};
    const TaskPtrVec secondlyInsertedTaskMocks{task3Mock};

    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    const mocks::NodeMockPtr node2Mock = std::make_shared<mocks::NodeMock>(2);
    NodePtrVec nodeMocks{node0Mock, node1Mock, node2Mock};

    const policy::NodeToTaskMapping mapping{{0, {task0Mock}}, {1, {task1Mock}}, {2, {task2Mock}}};
    const mapping::Difference difference{};

    EXPECT_CALL(*policyMock, buildNodeToTaskMapping(firstlyInsertedTaskMocks)).WillOnce(Return(mapping));
    EXPECT_CALL(*differenceCalculatorMock, calculate(mapping)).WillOnce(Return(difference));
    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*node0Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node1Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node2Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node0Mock, assign(std::static_pointer_cast<Task>(task0Mock)));
    EXPECT_CALL(*node1Mock, assign(std::static_pointer_cast<Task>(task1Mock)));
    EXPECT_CALL(*node2Mock, assign(std::static_pointer_cast<Task>(task2Mock)));
    sut.scheduleNewTasks(firstlyInsertedTaskMocks);

    ASSERT_FALSE(sut.areAnyTasksWaiting());

    const policy::NodeToTaskMapping newMapping{{0, {task3Mock}}, {1, {task0Mock}}, {2, {task2Mock, task1Mock}}};
    mapping::Difference newDifference;
    newDifference.migrations.emplace_back(0, 1, task0Mock);
    newDifference.preemptions.emplace_back(1, task1Mock);
    newDifference.nodeIdsWithoutChange.emplace_back(2);

    EXPECT_CALL(*policyMock, buildNodeToTaskMapping(secondlyInsertedTaskMocks)).WillOnce(Return(newMapping));
    EXPECT_CALL(*differenceCalculatorMock, calculate(newMapping)).WillOnce(Return(newDifference));
    expectGettingNodes(nodeMocks);
    {
        Sequence checkThatNothingGetsOverwritten;
        EXPECT_CALL(*node1Mock, extractTask()).WillOnce(Return(task1Mock));
        EXPECT_CALL(*task1Mock, performPreemption());
        EXPECT_CALL(*node0Mock, extractTask()).WillOnce(Return(task0Mock));
        EXPECT_CALL(*task0Mock, performMigration());
        EXPECT_CALL(*node1Mock, assign(std::static_pointer_cast<Task>(task0Mock)));

        EXPECT_CALL(*node0Mock, isIdle()).WillOnce(Return(true));
        EXPECT_CALL(*node1Mock, isIdle()).WillOnce(Return(false));
        EXPECT_CALL(*node2Mock, isIdle()).WillOnce(Return(false));
        EXPECT_CALL(*node0Mock, assign(std::static_pointer_cast<Task>(task3Mock)));
    }
    sut.scheduleNewTasks(secondlyInsertedTaskMocks);

    ASSERT_TRUE(sut.areAnyTasksWaiting());

    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*node0Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node1Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node2Mock, isIdle()).WillOnce(Return(true));
    EXPECT_CALL(*node0Mock, assign(_)).Times(0);
    EXPECT_CALL(*node1Mock, assign(_)).Times(0);
    EXPECT_CALL(*node2Mock, assign(std::static_pointer_cast<Task>(task1Mock)));
    sut.scheduleWaitingTasks();

    ASSERT_FALSE(sut.areAnyTasksWaiting());
}

} // namespace tests
} // namespace loadbalancer
} // namespace cloud
