#include <memory>

#include <gtest/gtest.h>

#include "Cloud/LoadBalancer/Policy/ShortestRemainingTimeFirst/ShortestRemainingTimeFirstWithMigrationsAndPreemptions.hpp"
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
namespace shortestremainingtimefirst
{
namespace tests
{

using testing::_;
using testing::Return;
using testing::ReturnRef;

struct ShortestRemainingTimeFirstWithMigrationsAndPreemptionsShould
    : cloud::loadbalancer::policy::tests::PolicyBaseFixture
{
    std::unique_ptr<ShortestRemainingTimeFirstWithMigrationsAndPreemptions> sut;
};

TEST_F(ShortestRemainingTimeFirstWithMigrationsAndPreemptionsShould,
       ScheduleTakingCurrentAndPredictedNodesStateIntoAccount)
{
    sut = std::make_unique<ShortestRemainingTimeFirstWithMigrationsAndPreemptions>(
        infrastructureMock, std::make_shared<logger::Logger>("", false), true);
    const mocks::TaskMockPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const mocks::TaskMockPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const mocks::TaskMockPtr task2Mock = std::make_shared<mocks::TaskMock>(2);
    const mocks::TaskMockPtr task3Mock = std::make_shared<mocks::TaskMock>(3);
    const mocks::TaskMockPtr task4Mock = std::make_shared<mocks::TaskMock>(4);
    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    const std::vector<NodePtr> nodeMocks{node0Mock, node1Mock};

    const NodeToTaskMapping expectedNodeToTaskMapping{{0, {task3Mock, task4Mock, task1Mock}},
                                                      {1, {task2Mock, task0Mock}}};

    EXPECT_CALL(*task0Mock, estimateTimeLeft()).WillRepeatedly(Return(10));
    EXPECT_CALL(*task1Mock, estimateTimeLeft()).WillRepeatedly(Return(15));
    EXPECT_CALL(*task2Mock, estimateTimeLeft()).WillRepeatedly(Return(5));
    EXPECT_CALL(*task3Mock, estimateTimeLeft()).WillRepeatedly(Return(4));
    EXPECT_CALL(*task4Mock, estimateTimeLeft()).WillRepeatedly(Return(6));
    EXPECT_CALL(*node0Mock, canTaskFit(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*node1Mock, canTaskFit(_)).WillRepeatedly(Return(true));
    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*node0Mock, getTask()).WillRepeatedly(Return(task0Mock));
    EXPECT_CALL(*node1Mock, getTask()).WillRepeatedly(Return(task1Mock));
    ASSERT_EQ(sut->buildNodeToTaskMapping({task2Mock, task3Mock, task4Mock}), expectedNodeToTaskMapping);
}

TEST_F(ShortestRemainingTimeFirstWithMigrationsAndPreemptionsShould, NotMigrateLongerTaskIfNewShorterCanFitOnOtherNode)
{
    sut = std::make_unique<ShortestRemainingTimeFirstWithMigrationsAndPreemptions>(
        infrastructureMock, std::make_shared<logger::Logger>("", false), true);
    const mocks::TaskMockPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const mocks::TaskMockPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const mocks::TaskMockPtr task2Mock = std::make_shared<mocks::TaskMock>(2);
    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    const std::vector<NodePtr> nodeMocks{node0Mock, node1Mock};

    const NodeToTaskMapping expectedFirstNodeToTaskMapping{{0, {task0Mock}}, {1, {task1Mock}}};
    const NodeToTaskMapping expectedSecondNodeToTaskMapping{{0, {task0Mock}}, {1, {task2Mock, task1Mock}}};

    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*task0Mock, estimateTimeLeft()).WillRepeatedly(Return(10));
    EXPECT_CALL(*task1Mock, estimateTimeLeft()).WillRepeatedly(Return(15));
    EXPECT_CALL(*node0Mock, canTaskFit(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*node1Mock, canTaskFit(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*node0Mock, getTask()).Times(2).WillOnce(Return(nullptr)).WillOnce(Return(nullptr));
    EXPECT_CALL(*node1Mock, getTask()).Times(2).WillOnce(Return(nullptr)).WillOnce(Return(nullptr));
    ASSERT_EQ(sut->buildNodeToTaskMapping({task0Mock, task1Mock}), expectedFirstNodeToTaskMapping);

    EXPECT_CALL(*task2Mock, estimateTimeLeft()).WillRepeatedly(Return(9));
    EXPECT_CALL(*node0Mock, getTask()).Times(2).WillOnce(Return(task0Mock)).WillOnce(Return(task0Mock));
    EXPECT_CALL(*node1Mock, getTask()).Times(2).WillOnce(Return(task1Mock)).WillOnce(Return(task1Mock));
    ASSERT_EQ(sut->buildNodeToTaskMapping({task2Mock}), expectedSecondNodeToTaskMapping);
}

TEST_F(ShortestRemainingTimeFirstWithMigrationsAndPreemptionsShould,
       NotMigrateLongestTaskIfShorterCannotFitOnOtherNodeAndLongerCanFitOnOtherNode)
{
    sut = std::make_unique<ShortestRemainingTimeFirstWithMigrationsAndPreemptions>(
        infrastructureMock, std::make_shared<logger::Logger>("", false), true);
    const mocks::TaskMockPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const mocks::TaskMockPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const mocks::TaskMockPtr task2Mock = std::make_shared<mocks::TaskMock>(2);
    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    const mocks::NodeMockPtr node2Mock = std::make_shared<mocks::NodeMock>(2);
    const std::vector<NodePtr> nodeMocks{node0Mock, node1Mock, node2Mock};

    const NodeToTaskMapping expectedFirstNodeToTaskMapping{{0, {task0Mock}}, {1, {task1Mock}}};
    const NodeToTaskMapping expectedSecondNodeToTaskMapping{{0, {task2Mock}}, {1, {task1Mock}}, {2, {task0Mock}}};

    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*task0Mock, estimateTimeLeft()).WillRepeatedly(Return(10));
    EXPECT_CALL(*task1Mock, estimateTimeLeft()).WillRepeatedly(Return(15));
    EXPECT_CALL(*node0Mock, canTaskFit(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*node1Mock, canTaskFit(std::static_pointer_cast<Task>(task0Mock))).WillRepeatedly(Return(true));
    EXPECT_CALL(*node1Mock, canTaskFit(std::static_pointer_cast<Task>(task1Mock))).WillRepeatedly(Return(true));
    EXPECT_CALL(*node1Mock, canTaskFit(std::static_pointer_cast<Task>(task2Mock))).WillRepeatedly(Return(false));
    EXPECT_CALL(*node2Mock, canTaskFit(std::static_pointer_cast<Task>(task0Mock))).WillRepeatedly(Return(true));
    EXPECT_CALL(*node2Mock, canTaskFit(std::static_pointer_cast<Task>(task1Mock))).WillRepeatedly(Return(true));
    EXPECT_CALL(*node2Mock, canTaskFit(std::static_pointer_cast<Task>(task2Mock))).WillRepeatedly(Return(false));
    EXPECT_CALL(*node0Mock, getTask()).Times(2).WillOnce(Return(nullptr)).WillOnce(Return(nullptr));
    EXPECT_CALL(*node1Mock, getTask()).Times(2).WillOnce(Return(nullptr)).WillOnce(Return(nullptr));
    EXPECT_CALL(*node2Mock, getTask()).Times(2).WillOnce(Return(nullptr)).WillOnce(Return(nullptr));
    ASSERT_EQ(sut->buildNodeToTaskMapping({task0Mock, task1Mock}), expectedFirstNodeToTaskMapping);

    EXPECT_CALL(*task2Mock, estimateTimeLeft()).WillRepeatedly(Return(9));
    EXPECT_CALL(*node0Mock, getTask()).Times(2).WillOnce(Return(task0Mock)).WillOnce(Return(task0Mock));
    EXPECT_CALL(*node1Mock, getTask()).Times(2).WillOnce(Return(task1Mock)).WillOnce(Return(task1Mock));
    EXPECT_CALL(*node2Mock, getTask()).Times(2).WillOnce(Return(nullptr)).WillOnce(Return(nullptr));
    ASSERT_EQ(sut->buildNodeToTaskMapping({task2Mock}), expectedSecondNodeToTaskMapping);
}

TEST_F(ShortestRemainingTimeFirstWithMigrationsAndPreemptionsShould, MigrateLongerTaskIfShorterCannotFitOnOtherNode)
{
    sut = std::make_unique<ShortestRemainingTimeFirstWithMigrationsAndPreemptions>(
        infrastructureMock, std::make_shared<logger::Logger>("", false), true);
    const mocks::TaskMockPtr task0Mock = std::make_shared<mocks::TaskMock>(0);
    const mocks::TaskMockPtr task1Mock = std::make_shared<mocks::TaskMock>(1);
    const mocks::TaskMockPtr task2Mock = std::make_shared<mocks::TaskMock>(2);
    const mocks::NodeMockPtr node0Mock = std::make_shared<mocks::NodeMock>(0);
    const mocks::NodeMockPtr node1Mock = std::make_shared<mocks::NodeMock>(1);
    const std::vector<NodePtr> nodeMocks{node0Mock, node1Mock};

    const NodeToTaskMapping expectedFirstNodeToTaskMapping{{0, {task0Mock}}, {1, {task1Mock}}};
    const NodeToTaskMapping expectedSecondNodeToTaskMapping{{0, {task2Mock, task1Mock}}, {1, {task0Mock}}};

    expectGettingNodes(nodeMocks);
    EXPECT_CALL(*task0Mock, estimateTimeLeft()).WillRepeatedly(Return(10));
    EXPECT_CALL(*task1Mock, estimateTimeLeft()).WillRepeatedly(Return(15));
    EXPECT_CALL(*node0Mock, canTaskFit(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*node1Mock, canTaskFit(std::static_pointer_cast<Task>(task0Mock))).WillRepeatedly(Return(true));
    EXPECT_CALL(*node1Mock, canTaskFit(std::static_pointer_cast<Task>(task1Mock))).WillRepeatedly(Return(true));
    EXPECT_CALL(*node1Mock, canTaskFit(std::static_pointer_cast<Task>(task2Mock))).WillRepeatedly(Return(false));
    EXPECT_CALL(*node0Mock, getTask()).Times(2).WillOnce(Return(nullptr)).WillOnce(Return(nullptr));
    EXPECT_CALL(*node1Mock, getTask()).Times(2).WillOnce(Return(nullptr)).WillOnce(Return(nullptr));
    ASSERT_EQ(sut->buildNodeToTaskMapping({task0Mock, task1Mock}), expectedFirstNodeToTaskMapping);

    EXPECT_CALL(*task2Mock, estimateTimeLeft()).WillRepeatedly(Return(9));
    EXPECT_CALL(*node0Mock, getTask()).Times(2).WillOnce(Return(task0Mock)).WillOnce(Return(task0Mock));
    EXPECT_CALL(*node1Mock, getTask()).Times(2).WillOnce(Return(task1Mock)).WillOnce(Return(task1Mock));
    ASSERT_EQ(sut->buildNodeToTaskMapping({task2Mock}), expectedSecondNodeToTaskMapping);
}

} // namespace tests
} // namespace shortestremainingtimefirst
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
