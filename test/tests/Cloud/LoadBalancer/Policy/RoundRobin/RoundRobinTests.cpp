#include <gtest/gtest.h>
#include <memory>

#include "Cloud/LoadBalancer/Policy/RoundRobin/RoundRobin.hpp"
#include "mocks/NodeMock.hpp"
#include "mocks/TaskMock.hpp"
#include "tests/Cloud/LoadBalancer/Policy/PolicyBaseFixture.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace roundrobin
{
namespace tests
{

using testing::_;
using testing::Return;

struct RoundRobinShould : cloud::loadbalancer::policy::tests::PolicyBaseFixture
{
    RoundRobin sut{infrastructureMock, std::make_shared<logger::Logger>("", false)};
};

TEST_F(RoundRobinShould, TakePreviousMappingIntoAccount)
{
    const mocks::TaskMockPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const mocks::TaskMockPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const mocks::TaskMockPtr task2Mock = std::make_shared<mocks::TaskMock>(2);
    const mocks::TaskMockPtr task3Mock = std::make_shared<mocks::TaskMock>(3);
    const mocks::TaskMockPtr task4Mock = std::make_shared<mocks::TaskMock>(4);
    const mocks::TaskMockPtr task5Mock = std::make_shared<mocks::TaskMock>(5);
    const mocks::TaskMockPtr task6Mock = std::make_shared<mocks::TaskMock>(6);
    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    const mocks::NodeMockPtr node2Mock = std::make_shared<mocks::NodeMock>(2);
    const std::vector<NodePtr> nodeMocks{node0Mock, node1Mock, node2Mock};
    const NodeToTaskMapping expectedFirstNodeToTaskMapping{
        {0, {task0Mock, task3Mock}}, {1, {task1Mock, task4Mock}}, {2, {task2Mock, task5Mock}}};
    const NodeToTaskMapping expectedSecondNodeToTaskMapping{
        {0, {task6Mock}}, {1, {task4Mock}}, {2, {task2Mock, task5Mock}}};

    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*node0Mock, canTaskFit(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*node1Mock, canTaskFit(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*node2Mock, canTaskFit(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*node0Mock, getTask()).WillOnce(Return(nullptr));
    EXPECT_CALL(*node1Mock, getTask()).WillOnce(Return(nullptr));
    EXPECT_CALL(*node2Mock, getTask()).WillOnce(Return(nullptr));
    ASSERT_EQ(sut.buildNodeToTaskMapping({task0Mock, task1Mock, task2Mock, task3Mock, task4Mock, task5Mock}),
              expectedFirstNodeToTaskMapping);

    EXPECT_CALL(*node0Mock, getTask()).WillOnce(Return(nullptr));
    EXPECT_CALL(*node1Mock, getTask()).WillOnce(Return(task4Mock));
    EXPECT_CALL(*node2Mock, getTask()).WillOnce(Return(task2Mock));
    ASSERT_EQ(sut.buildNodeToTaskMapping({task2Mock, task4Mock, task5Mock, task6Mock}),
              expectedSecondNodeToTaskMapping);
}

TEST_F(RoundRobinShould, ResumeMappingFromLastlySelectedNode)
{
    const mocks::TaskMockPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const mocks::TaskMockPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const mocks::TaskMockPtr task2Mock = std::make_shared<mocks::TaskMock>(2);
    const mocks::TaskMockPtr task3Mock = std::make_shared<mocks::TaskMock>(3);
    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    const std::vector<NodePtr> nodeMocks{node0Mock, node1Mock};
    const NodeToTaskMapping expectedFirstNodeToTaskMapping{{0, {task0Mock, task2Mock}}, {1, {task1Mock}}};
    const NodeToTaskMapping expectedSecondNodeToTaskMapping{{0, {}}, {1, {task3Mock}}};

    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*node0Mock, canTaskFit(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*node1Mock, canTaskFit(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*node0Mock, getTask()).WillOnce(Return(nullptr));
    EXPECT_CALL(*node1Mock, getTask()).WillOnce(Return(nullptr));
    ASSERT_EQ(sut.buildNodeToTaskMapping({task0Mock, task1Mock, task2Mock}), expectedFirstNodeToTaskMapping);

    EXPECT_CALL(*node0Mock, getTask()).WillOnce(Return(nullptr));
    EXPECT_CALL(*node1Mock, getTask()).WillOnce(Return(nullptr));
    ASSERT_EQ(sut.buildNodeToTaskMapping({task3Mock}), expectedSecondNodeToTaskMapping);
}

TEST_F(RoundRobinShould, ScheduleNotTakingNotFeasibleNodesIntoAccount)
{
    const mocks::TaskMockPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const mocks::TaskMockPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const mocks::TaskMockPtr task2Mock = std::make_shared<mocks::TaskMock>(2);
    const mocks::TaskMockPtr task3Mock = std::make_shared<mocks::TaskMock>(3);
    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    const mocks::NodeMockPtr node2Mock = std::make_shared<mocks::NodeMock>(2);
    const std::vector<NodePtr> nodeMocks{node0Mock, node1Mock, node2Mock};
    const NodeToTaskMapping expectedNodeToTaskMapping{{0, {task0Mock}}, {1, {task2Mock}}, {2, {task1Mock, task3Mock}}};

    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*node0Mock, getTask()).WillOnce(Return(nullptr));
    EXPECT_CALL(*node1Mock, getTask()).WillOnce(Return(nullptr));
    EXPECT_CALL(*node2Mock, getTask()).WillOnce(Return(nullptr));
    EXPECT_CALL(*node0Mock, canTaskFit(std::static_pointer_cast<Task>(task0Mock))).WillOnce(Return(true));
    EXPECT_CALL(*node1Mock, canTaskFit(std::static_pointer_cast<Task>(task1Mock))).WillOnce(Return(false));
    EXPECT_CALL(*node2Mock, canTaskFit(std::static_pointer_cast<Task>(task1Mock))).WillOnce(Return(true));
    EXPECT_CALL(*node0Mock, canTaskFit(std::static_pointer_cast<Task>(task2Mock))).WillOnce(Return(false));
    EXPECT_CALL(*node1Mock, canTaskFit(std::static_pointer_cast<Task>(task2Mock))).WillOnce(Return(true));
    EXPECT_CALL(*node2Mock, canTaskFit(std::static_pointer_cast<Task>(task3Mock))).WillOnce(Return(true));
    ASSERT_EQ(sut.buildNodeToTaskMapping({task0Mock, task1Mock, task2Mock, task3Mock}), expectedNodeToTaskMapping);
}

} // namespace tests
} // namespace roundrobin
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
