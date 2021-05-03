#include <memory>

#include <gtest/gtest.h>

#include "Cloud/LoadBalancer/Policy/LongestRemainingTimeFirst/LongestRemainingTimeFirstWithMigrationsAndPreemptions.hpp"
#include "mocks/InfrastructureMock.hpp"
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
using testing::ReturnRef;

struct LongestRemainingTimeFirstWithMigrationsAndPreemptionsShould
    : cloud::loadbalancer::policy::tests::PolicyBaseFixture
{
    LongestRemainingTimeFirstWithMigrationsAndPreemptions sut{infrastructureMock,
                                                              std::make_shared<logger::Logger>("", false), true};
};

TEST_F(LongestRemainingTimeFirstWithMigrationsAndPreemptionsShould,
       ScheduleTakingCurrentAndPredictedNodesStateIntoAccount)
{
    const mocks::TaskMockPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const mocks::TaskMockPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const mocks::TaskMockPtr task2Mock = std::make_shared<mocks::TaskMock>(2);
    const mocks::TaskMockPtr task3Mock = std::make_shared<mocks::TaskMock>(3);
    const mocks::TaskMockPtr task4Mock = std::make_shared<mocks::TaskMock>(4);
    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    const std::vector<NodePtr> nodeMocks{node0Mock, node1Mock};

    const NodeToTaskMapping expectedNodeToTaskMapping{{0, {task3Mock, task0Mock}},
                                                      {1, {task4Mock, task2Mock, task1Mock}}};

    EXPECT_CALL(*task0Mock, estimateTimeLeft()).WillRepeatedly(Return(15));
    EXPECT_CALL(*task1Mock, estimateTimeLeft()).WillRepeatedly(Return(10));
    EXPECT_CALL(*task2Mock, estimateTimeLeft()).WillRepeatedly(Return(12));
    EXPECT_CALL(*task3Mock, estimateTimeLeft()).WillRepeatedly(Return(16));
    EXPECT_CALL(*task4Mock, estimateTimeLeft()).WillRepeatedly(Return(16));
    EXPECT_CALL(*node0Mock, canTaskFit(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*node1Mock, canTaskFit(_)).WillRepeatedly(Return(true));
    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*node0Mock, getTask()).WillRepeatedly(Return(task0Mock));
    EXPECT_CALL(*node1Mock, getTask()).WillRepeatedly(Return(task1Mock));
    ASSERT_EQ(sut.buildNodeToTaskMapping({task4Mock, task3Mock, task2Mock}), expectedNodeToTaskMapping);
}

TEST_F(LongestRemainingTimeFirstWithMigrationsAndPreemptionsShould, ScheduleNotTakingNotFeasibleNodesIntoAccount)
{
    const mocks::TaskMockPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const mocks::TaskMockPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const mocks::TaskMockPtr task2Mock = std::make_shared<mocks::TaskMock>(2);
    const mocks::TaskMockPtr task3Mock = std::make_shared<mocks::TaskMock>(3);
    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    const std::vector<NodePtr> nodeMocks{node0Mock, node1Mock};

    const NodeToTaskMapping expectedNodeToTaskMapping{{0, {task3Mock}}, {1, {task1Mock, task0Mock, task2Mock}}};

    EXPECT_CALL(*task0Mock, estimateTimeLeft()).WillRepeatedly(Return(8));
    EXPECT_CALL(*task1Mock, estimateTimeLeft()).WillRepeatedly(Return(9));
    EXPECT_CALL(*task2Mock, estimateTimeLeft()).WillRepeatedly(Return(4));
    EXPECT_CALL(*task3Mock, estimateTimeLeft()).WillRepeatedly(Return(10));
    EXPECT_CALL(*node0Mock, canTaskFit(std::static_pointer_cast<Task>(task0Mock))).WillOnce(Return(true));
    EXPECT_CALL(*node0Mock, canTaskFit(std::static_pointer_cast<Task>(task1Mock))).WillOnce(Return(true));
    EXPECT_CALL(*node0Mock, canTaskFit(std::static_pointer_cast<Task>(task2Mock))).WillOnce(Return(false));
    EXPECT_CALL(*node0Mock, canTaskFit(std::static_pointer_cast<Task>(task3Mock))).WillOnce(Return(true));
    EXPECT_CALL(*node1Mock, canTaskFit(_)).WillRepeatedly(Return(true));
    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*node0Mock, getTask()).WillRepeatedly(Return(task0Mock));
    EXPECT_CALL(*node1Mock, getTask()).WillRepeatedly(Return(task1Mock));
    ASSERT_EQ(sut.buildNodeToTaskMapping({task2Mock, task3Mock}), expectedNodeToTaskMapping);
}

} // namespace tests
} // namespace longestremainingtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
