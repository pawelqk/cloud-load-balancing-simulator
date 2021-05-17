#include <memory>

#include <gtest/gtest.h>

#include "Cloud/LoadBalancer/Policy/LongestRemainingTimeFirst/LongestRemainingTimeFirst.hpp"
#include "mocks/NodeMock.hpp"
#include "mocks/TaskMock.hpp"
#include "tests/Cloud/LoadBalancer/Policy/PolicyBaseFixture.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace longestremainingtimefirst
{
namespace tests
{

using testing::_;
using testing::Return;

struct LongestRemainingTimeFirstShould : cloud::loadbalancer::policy::tests::PolicyBaseFixture
{
    LongestRemainingTimeFirst sut{infrastructureMock, std::make_shared<logger::Logger>("", false)};
};

TEST_F(LongestRemainingTimeFirstShould, ScheduleTakingCurrentAndPredictedNodesStateIntoAccount)
{
    const mocks::TaskMockPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const mocks::TaskMockPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const mocks::TaskMockPtr task2Mock = std::make_shared<mocks::TaskMock>(2);
    const mocks::TaskMockPtr task3Mock = std::make_shared<mocks::TaskMock>(3);
    const mocks::TaskMockPtr task4Mock = std::make_shared<mocks::TaskMock>(4);
    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    const std::vector<NodePtr> nodeMocks{node0Mock, node1Mock};

    const NodeToTaskMapping expectedNodeToTaskMapping{{0, {task0Mock, task3Mock, task2Mock}},
                                                      {1, {task1Mock, task4Mock}}};

    EXPECT_CALL(*task0Mock, estimateTimeLeft()).WillRepeatedly(Return(6));
    EXPECT_CALL(*task1Mock, estimateTimeLeft()).WillRepeatedly(Return(20));
    EXPECT_CALL(*task2Mock, estimateTimeLeft()).WillRepeatedly(Return(10));
    EXPECT_CALL(*task3Mock, estimateTimeLeft()).WillRepeatedly(Return(11));
    EXPECT_CALL(*task4Mock, estimateTimeLeft()).WillRepeatedly(Return(9));
    EXPECT_CALL(*node0Mock, canTaskFit(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*node1Mock, canTaskFit(_)).WillRepeatedly(Return(true));
    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*node0Mock, getTask()).Times(2).WillOnce(Return(task0Mock)).WillOnce(Return(task0Mock));
    EXPECT_CALL(*node1Mock, getTask()).Times(2).WillOnce(Return(task1Mock)).WillOnce(Return(task1Mock));
    ASSERT_EQ(sut.buildNodeToTaskMapping({task2Mock, task3Mock, task4Mock}), expectedNodeToTaskMapping);
}

TEST_F(LongestRemainingTimeFirstShould, ScheduleNotTakingNotFeasibleNodesIntoAccount)
{
    const mocks::TaskMockPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const mocks::TaskMockPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const mocks::TaskMockPtr task2Mock = std::make_shared<mocks::TaskMock>(2);
    const mocks::TaskMockPtr task3Mock = std::make_shared<mocks::TaskMock>(3);
    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    const std::vector<NodePtr> nodeMocks{node0Mock, node1Mock};

    const NodeToTaskMapping expectedNodeToTaskMapping{{0, {task0Mock, task2Mock}}, {1, {task1Mock, task3Mock}}};

    EXPECT_CALL(*task0Mock, estimateTimeLeft()).WillRepeatedly(Return(10));
    EXPECT_CALL(*task1Mock, estimateTimeLeft()).WillRepeatedly(Return(15));
    EXPECT_CALL(*task2Mock, estimateTimeLeft()).WillRepeatedly(Return(4));
    EXPECT_CALL(*task3Mock, estimateTimeLeft()).WillRepeatedly(Return(5));
    EXPECT_CALL(*node0Mock, canTaskFit(std::static_pointer_cast<Task>(task3Mock))).WillOnce(Return(false));
    EXPECT_CALL(*node0Mock, canTaskFit(std::static_pointer_cast<Task>(task2Mock))).WillOnce(Return(true));
    EXPECT_CALL(*node1Mock, canTaskFit(_)).WillRepeatedly(Return(true));
    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*node0Mock, getTask()).Times(2).WillOnce(Return(task0Mock)).WillOnce(Return(task0Mock));
    EXPECT_CALL(*node1Mock, getTask()).Times(2).WillOnce(Return(task1Mock)).WillOnce(Return(task1Mock));
    ASSERT_EQ(sut.buildNodeToTaskMapping({task2Mock, task3Mock}), expectedNodeToTaskMapping);
}

} // namespace tests
} // namespace longestremainingtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
