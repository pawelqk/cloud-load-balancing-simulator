#include <gtest/gtest.h>

#include "Cloud/LoadBalancer/Mapping/MakespanAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/Policy.hpp"
#include "mocks/DifferenceCalculatorMock.hpp"
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

using testing::Return;
using testing::ReturnRef;

struct MakespanAssessorShould : testing::Test
{
    const mocks::DifferenceCalculatorMockPtr differenceCalculatorMock{
        std::make_shared<mocks::DifferenceCalculatorMock>()};

    void expectNotChangedTaskCall(const cloud::mocks::TaskMockPtr &taskMock, const std::uint32_t timeLeft)
    {
        EXPECT_CALL(*taskMock, estimateTimeLeft()).WillOnce(Return(timeLeft));
    }

    void expectPreemptedTaskCall(const cloud::mocks::TaskMockPtr &taskMock, const std::uint32_t timeLeft)
    {
        EXPECT_CALL(*taskMock, estimateTimeLeftAfterPreemption()).WillOnce(Return(timeLeft));
    }

    void expectMigratedTaskCall(const cloud::mocks::TaskMockPtr &taskMock, const std::uint32_t timeLeft)
    {
        EXPECT_CALL(*taskMock, estimateTimeLeftAfterMigration()).WillOnce(Return(timeLeft));
    }

    MakespanAssessor sut{differenceCalculatorMock};
};

TEST_F(MakespanAssessorShould, CalculateMakespanForMultipleNodes)
{
    const cloud::mocks::TaskMockPtr taskMock0 = std::make_shared<cloud::mocks::TaskMock>(0);
    const cloud::mocks::TaskMockPtr taskMock1 = std::make_shared<cloud::mocks::TaskMock>(1);
    const cloud::mocks::TaskMockPtr taskMock2 = std::make_shared<cloud::mocks::TaskMock>(2);
    const cloud::mocks::TaskMockPtr taskMock3 = std::make_shared<cloud::mocks::TaskMock>(3);
    const cloud::mocks::TaskMockPtr taskMock4 = std::make_shared<cloud::mocks::TaskMock>(4);
    const policy::NodeToTaskMapping mapping{{1, {taskMock0, taskMock1, taskMock2}}, {2, {taskMock3, taskMock4}}};

    constexpr auto task0Makespan = 1;
    expectNotChangedTaskCall(taskMock0, task0Makespan);

    constexpr auto task1Makespan = 5;
    expectNotChangedTaskCall(taskMock1, task1Makespan);

    constexpr auto task2Makespan = 6;
    expectNotChangedTaskCall(taskMock2, task2Makespan);

    constexpr auto task3Makespan = 11;
    expectNotChangedTaskCall(taskMock3, task3Makespan);

    constexpr auto task4Makespan = 2;
    expectNotChangedTaskCall(taskMock4, task4Makespan);

    EXPECT_CALL(*differenceCalculatorMock, calculate(mapping)).WillOnce(Return(Difference{}));
    ASSERT_EQ(sut.assess(mapping), task3Makespan + task4Makespan);
}

TEST_F(MakespanAssessorShould, CalculateMakespanConsideringPreemptions)
{
    constexpr auto placeholderSourceNodeId{100};

    const cloud::mocks::TaskMockPtr taskMock0 = std::make_shared<cloud::mocks::TaskMock>(0);
    const cloud::mocks::TaskMockPtr taskMock1 = std::make_shared<cloud::mocks::TaskMock>(1);
    const cloud::mocks::TaskMockPtr taskMock2 = std::make_shared<cloud::mocks::TaskMock>(2);
    const cloud::mocks::TaskMockPtr taskMock3 = std::make_shared<cloud::mocks::TaskMock>(2);
    const policy::NodeToTaskMapping mapping{{1, {taskMock0, taskMock2}}, {2, {taskMock1, taskMock3}}};
    Difference diffWithPreemptions;
    diffWithPreemptions.preemptions.emplace_back(placeholderSourceNodeId, taskMock2);
    diffWithPreemptions.preemptions.emplace_back(placeholderSourceNodeId, taskMock3);

    constexpr auto task0Makespan = 4;
    expectNotChangedTaskCall(taskMock0, task0Makespan);

    constexpr auto task1Makespan = 6;
    expectNotChangedTaskCall(taskMock1, task1Makespan);

    constexpr auto task2Makespan = 7;
    expectPreemptedTaskCall(taskMock2, task2Makespan);

    constexpr auto task3Makespan = 8;
    expectPreemptedTaskCall(taskMock3, task3Makespan);

    EXPECT_CALL(*differenceCalculatorMock, calculate(mapping)).WillOnce(Return(diffWithPreemptions));
    ASSERT_EQ(sut.assess(mapping), task1Makespan + task3Makespan);
}

TEST_F(MakespanAssessorShould, CalculateMakespanConsideringMigrations)
{
    constexpr auto placeholderSourceNodeId{100};
    constexpr auto placeholderDestinationNodeId{200};

    const cloud::mocks::TaskMockPtr taskMock0 = std::make_shared<cloud::mocks::TaskMock>(0);
    const cloud::mocks::TaskMockPtr taskMock1 = std::make_shared<cloud::mocks::TaskMock>(1);
    const cloud::mocks::TaskMockPtr taskMock2 = std::make_shared<cloud::mocks::TaskMock>(2);
    const cloud::mocks::TaskMockPtr taskMock3 = std::make_shared<cloud::mocks::TaskMock>(2);
    const policy::NodeToTaskMapping mapping{{1, {taskMock0, taskMock2}}, {2, {taskMock1, taskMock3}}};
    Difference diffWithMigrations;
    diffWithMigrations.migrations.emplace_back(placeholderSourceNodeId, placeholderDestinationNodeId, taskMock0);
    diffWithMigrations.migrations.emplace_back(placeholderSourceNodeId, placeholderDestinationNodeId, taskMock1);

    constexpr auto task0Makespan = 10;
    expectMigratedTaskCall(taskMock0, task0Makespan);

    constexpr auto task1Makespan = 3;
    expectMigratedTaskCall(taskMock1, task1Makespan);

    constexpr auto task2Makespan = 10;
    expectNotChangedTaskCall(taskMock2, task2Makespan);

    constexpr auto task3Makespan = 2;
    expectNotChangedTaskCall(taskMock3, task3Makespan);

    EXPECT_CALL(*differenceCalculatorMock, calculate(mapping)).WillOnce(Return(diffWithMigrations));
    ASSERT_EQ(sut.assess(mapping), task0Makespan + task2Makespan);
}

TEST_F(MakespanAssessorShould, CalculateMakespanConsideringEverythingAtOnce)
{
    constexpr auto placeholderSourceNodeId{100};
    constexpr auto placeholderDestinationNodeId{200};

    const cloud::mocks::TaskMockPtr taskMock0 = std::make_shared<cloud::mocks::TaskMock>(0);
    const cloud::mocks::TaskMockPtr taskMock1 = std::make_shared<cloud::mocks::TaskMock>(1);
    const cloud::mocks::TaskMockPtr taskMock2 = std::make_shared<cloud::mocks::TaskMock>(2);
    const policy::NodeToTaskMapping mapping{{1, {taskMock0, taskMock2}}, {2, {taskMock1}}};
    Difference diffWithEverything;
    diffWithEverything.preemptions.emplace_back(placeholderSourceNodeId, taskMock0);
    diffWithEverything.migrations.emplace_back(placeholderSourceNodeId, placeholderDestinationNodeId, taskMock1);

    constexpr auto task0Makespan = 4;
    expectPreemptedTaskCall(taskMock0, task0Makespan);

    constexpr auto task1Makespan = 20;
    expectMigratedTaskCall(taskMock1, task1Makespan);

    constexpr auto task2Makespan = 7;
    expectNotChangedTaskCall(taskMock2, task2Makespan);

    EXPECT_CALL(*differenceCalculatorMock, calculate(mapping)).WillOnce(Return(diffWithEverything));
    ASSERT_EQ(sut.assess(mapping), task1Makespan);
}

} // namespace tests
} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
