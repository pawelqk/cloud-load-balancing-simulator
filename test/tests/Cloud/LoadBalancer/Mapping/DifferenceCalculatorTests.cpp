#include <gtest/gtest.h>

#include "Cloud/LoadBalancer/Mapping/DifferenceCalculatorImpl.hpp"
#include "Cloud/LoadBalancer/Policy/Policy.hpp"
#include "mocks/InfrastructureMock.hpp"
#include "mocks/NodeMock.hpp"
#include "mocks/TaskMock.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{
namespace tests
{

using testing::Field;
using testing::Return;
using testing::ReturnRef;
using testing::UnorderedElementsAreArray;

struct DifferenceCalculatorShould : testing::Test
{
    void expectGettingNodes(const std::vector<NodePtr> &nodeMocks)
    {
        EXPECT_CALL(Const(*infrastructureMock), getNodes()).WillOnce(ReturnRef(nodeMocks));
    }

    void expectCalculatedDifference(const policy::NodeToTaskMapping &mapping, const Difference &expectedDiff)
    {
        ASSERT_THAT(sut.calculate(mapping),
                    AllOf(Field(&Difference::migrations, UnorderedElementsAreArray(expectedDiff.migrations)),
                          Field(&Difference::preemptions, UnorderedElementsAreArray(expectedDiff.preemptions)),
                          Field(&Difference::nodeIdsWithoutChange,
                                UnorderedElementsAreArray(expectedDiff.nodeIdsWithoutChange))));
    }

    const mocks::InfrastructureMockPtr infrastructureMock{std::make_shared<mocks::InfrastructureMock>()};

    DifferenceCalculatorImpl sut{infrastructureMock};
};

TEST_F(DifferenceCalculatorShould, DetectNoChange)
{
    constexpr NodeId nodeId{2};
    const mocks::TaskMockPtr taskMock = std::make_shared<mocks::TaskMock>(0);
    const mocks::NodeMockPtr nodeMock = std::make_shared<mocks::NodeMock>(nodeId);

    // previous mapping: {{0, {nodeId}}}
    const policy::NodeToTaskMapping mapping{{nodeId, {taskMock}}};
    const std::vector<NodePtr> nodeMocks{nodeMock};
    Difference expectedDiff;
    expectedDiff.nodeIdsWithoutChange.push_back(nodeId);

    EXPECT_CALL(*nodeMock, getTask()).WillOnce(Return(taskMock));
    expectGettingNodes(nodeMocks);
    expectCalculatedDifference(mapping, expectedDiff);
}

TEST_F(DifferenceCalculatorShould, DetectTaskAssignments)
{
    const mocks::TaskMockPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const mocks::TaskMockPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);

    // previous mapping: {{0, {}}, {1, {}}}
    const policy::NodeToTaskMapping mapping{{0, {task1Mock}}, {1, {task0Mock}}};
    const std::vector<NodePtr> nodeMocks{node0Mock, node1Mock};
    Difference expectedDiff;

    EXPECT_CALL(*node0Mock, getTask()).WillOnce(Return(nullptr));
    EXPECT_CALL(*node1Mock, getTask()).WillOnce(Return(nullptr));
    expectGettingNodes(nodeMocks);
    expectCalculatedDifference(mapping, expectedDiff);
}

TEST_F(DifferenceCalculatorShould, DetectTaskMigrations)
{
    const mocks::TaskMockPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const mocks::TaskMockPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const mocks::TaskMockPtr task2Mock = std::make_shared<mocks::TaskMock>(2);
    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    const mocks::NodeMockPtr node2Mock = std::make_shared<mocks::NodeMock>(2);
    const mocks::NodeMockPtr node3Mock = std::make_shared<mocks::NodeMock>(3);

    // previous mapping: {{0, {task0Mock}}, {1, {task1Mock}}, {2, {task2Mock}}, {3, {}}}
    const policy::NodeToTaskMapping mapping{{0, {task1Mock}}, {1, {task0Mock}}, {3, {task2Mock}}};
    const std::vector<NodePtr> nodeMocks{node0Mock, node1Mock, node2Mock, node3Mock};
    Difference expectedDiff;
    expectedDiff.migrations.emplace_back(0, 1, task0Mock);
    expectedDiff.migrations.emplace_back(1, 0, task1Mock);
    expectedDiff.migrations.emplace_back(2, 3, task2Mock);

    EXPECT_CALL(*node0Mock, getTask()).WillOnce(Return(task0Mock));
    EXPECT_CALL(*node1Mock, getTask()).WillOnce(Return(task1Mock));
    EXPECT_CALL(*node2Mock, getTask()).WillOnce(Return(task2Mock));
    EXPECT_CALL(*node3Mock, getTask()).WillOnce(Return(nullptr));
    expectGettingNodes(nodeMocks);
    expectCalculatedDifference(mapping, expectedDiff);
}

TEST_F(DifferenceCalculatorShould, DetectTaskPreemptions)
{
    const mocks::TaskMockPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const mocks::TaskMockPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const mocks::TaskMockPtr task2Mock = std::make_shared<mocks::TaskMock>(2);
    const mocks::TaskMockPtr task3Mock = std::make_shared<mocks::TaskMock>(3);
    const mocks::TaskMockPtr task4Mock = std::make_shared<mocks::TaskMock>(4);
    const mocks::TaskMockPtr task5Mock = std::make_shared<mocks::TaskMock>(5);
    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    const mocks::NodeMockPtr node2Mock = std::make_shared<mocks::NodeMock>(2);
    const mocks::NodeMockPtr node3Mock = std::make_shared<mocks::NodeMock>(3);

    // previous mapping: {{0, {task0Mock, task1Mock}}, {1, {task2Mock, task3Mock}}, {2,
    // {task4Mock}}, {3, {task5Mock}}}
    const policy::NodeToTaskMapping mapping{
        {0, {task1Mock, task2Mock}}, {1, {task3Mock, task0Mock}}, {2, {task4Mock, task5Mock}}};
    const std::vector<NodePtr> nodeMocks{node0Mock, node1Mock, node2Mock, node3Mock};
    Difference expectedDiff;
    expectedDiff.preemptions.emplace_back(0, task0Mock);
    expectedDiff.preemptions.emplace_back(1, task2Mock);
    expectedDiff.preemptions.emplace_back(3, task5Mock);
    expectedDiff.nodeIdsWithoutChange.push_back(2);

    EXPECT_CALL(*node0Mock, getTask()).WillOnce(Return(task0Mock));
    EXPECT_CALL(*node1Mock, getTask()).WillOnce(Return(task2Mock));
    EXPECT_CALL(*node2Mock, getTask()).WillOnce(Return(task4Mock));
    EXPECT_CALL(*node3Mock, getTask()).WillOnce(Return(task5Mock));
    expectGettingNodes(nodeMocks);
    expectCalculatedDifference(mapping, expectedDiff);
}

TEST_F(DifferenceCalculatorShould, DetectEverythingAtOnce)
{
    const mocks::TaskMockPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const mocks::TaskMockPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const mocks::TaskMockPtr task2Mock = std::make_shared<mocks::TaskMock>(2);
    const mocks::TaskMockPtr task3Mock = std::make_shared<mocks::TaskMock>(3);
    const mocks::TaskMockPtr task4Mock = std::make_shared<mocks::TaskMock>(4);
    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    const mocks::NodeMockPtr node2Mock = std::make_shared<mocks::NodeMock>(2);
    const mocks::NodeMockPtr node3Mock = std::make_shared<mocks::NodeMock>(3);

    // previous mapping: {{0, {task0Mock}}, {1, {task1Mock, task2Mock}}, {2,
    // {task3Mock}}, {3, {}}}
    const policy::NodeToTaskMapping mapping{
        {0, {task1Mock}}, {1, {task2Mock, task0Mock}}, {2, {task3Mock}}, {3, {task4Mock}}};
    const std::vector<NodePtr> nodeMocks{node0Mock, node1Mock, node2Mock, node3Mock};
    Difference expectedDiff;
    expectedDiff.migrations.emplace_back(1, 0, task1Mock);
    expectedDiff.preemptions.emplace_back(0, task0Mock);
    expectedDiff.nodeIdsWithoutChange.push_back(2);

    EXPECT_CALL(*node0Mock, getTask()).WillOnce(Return(task0Mock));
    EXPECT_CALL(*node1Mock, getTask()).WillOnce(Return(task1Mock));
    EXPECT_CALL(*node2Mock, getTask()).WillOnce(Return(task3Mock));
    EXPECT_CALL(*node3Mock, getTask()).WillOnce(Return(nullptr));
    expectGettingNodes(nodeMocks);
    expectCalculatedDifference(mapping, expectedDiff);
}
} // namespace tests
} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
