#include <gtest/gtest.h>

#include "Cloud/LoadBalancer/Mapping/FlowtimeAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/Policy.hpp"
#include "mocks/DifferenceCalculatorMock.hpp"
#include "mocks/NodeMock.hpp"
#include "mocks/TaskMock.hpp"
#include "mocks/TimingServiceMock.hpp"

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

struct FlowtimeAssessorShould : testing::Test
{
    void expectNotChangedTaskCall(const cloud::mocks::TaskMockPtr &taskMock, const std::uint32_t arrivalTime,
                                  const std::uint32_t timeLeft)
    {
        EXPECT_CALL(*taskMock, estimateTimeLeft()).WillOnce(Return(timeLeft));
        EXPECT_CALL(*taskMock, getArrivalTime()).WillOnce(Return(arrivalTime));
    }

    void expectPreemptedTaskCall(const cloud::mocks::TaskMockPtr &taskMock, const std::uint32_t arrivalTime,
                                 const std::uint32_t timeLeft)
    {
        EXPECT_CALL(*taskMock, estimateTimeLeftAfterPreemption()).WillOnce(Return(timeLeft));
        EXPECT_CALL(*taskMock, getArrivalTime()).WillOnce(Return(arrivalTime));
    }

    void expectMigratedTaskCall(const cloud::mocks::TaskMockPtr &taskMock, const std::uint32_t arrivalTime,
                                const std::uint32_t timeLeft)
    {
        EXPECT_CALL(*taskMock, estimateTimeLeftAfterMigration()).WillOnce(Return(timeLeft));
        EXPECT_CALL(*taskMock, getArrivalTime()).WillOnce(Return(arrivalTime));
    }

    const mocks::DifferenceCalculatorMockPtr differenceCalculatorMock{
        std::make_shared<mocks::DifferenceCalculatorMock>()};
    const cloud::mocks::TimingServiceMockPtr timingServiceMock{std::make_shared<cloud::mocks::TimingServiceMock>()};

    FlowtimeAssessor sut{differenceCalculatorMock, timingServiceMock};
};

TEST_F(FlowtimeAssessorShould, CalculateFlowtimeForMultipleNodes)
{
    const cloud::mocks::TaskMockPtr taskMock0 = std::make_shared<cloud::mocks::TaskMock>(0);
    const cloud::mocks::TaskMockPtr taskMock1 = std::make_shared<cloud::mocks::TaskMock>(1);
    const cloud::mocks::TaskMockPtr taskMock2 = std::make_shared<cloud::mocks::TaskMock>(2);
    const cloud::mocks::TaskMockPtr taskMock3 = std::make_shared<cloud::mocks::TaskMock>(3);
    const cloud::mocks::TaskMockPtr taskMock4 = std::make_shared<cloud::mocks::TaskMock>(4);
    const policy::NodeToTaskMapping mapping{{1, {taskMock0, taskMock1, taskMock2}}, {2, {taskMock3, taskMock4}}};

    constexpr auto currentTime = 4;
    EXPECT_CALL(*timingServiceMock, getTicks()).WillRepeatedly(Return(currentTime));

    constexpr auto task0Flowtime = currentTime - 1 + 3;
    expectNotChangedTaskCall(taskMock0, 1, 3);

    constexpr auto task1Flowtime = task0Flowtime + currentTime - 2 + 4;
    expectNotChangedTaskCall(taskMock1, 2, 4);

    constexpr auto task2Flowtime = task0Flowtime + task1Flowtime + currentTime - 3 + 10;
    expectNotChangedTaskCall(taskMock2, 3, 10);

    constexpr auto task3Flowtime = currentTime - 3 + 5;
    expectNotChangedTaskCall(taskMock3, 3, 5);

    constexpr auto task4Flowtime = task3Flowtime + currentTime - 4 + 13;
    expectNotChangedTaskCall(taskMock4, 4, 13);

    EXPECT_CALL(*differenceCalculatorMock, calculate(mapping)).WillOnce(Return(Difference{}));
    ASSERT_EQ(sut.assess(mapping), task0Flowtime + task1Flowtime + task2Flowtime + task3Flowtime + task4Flowtime);
}

TEST_F(FlowtimeAssessorShould, CalculateFlowtimeConsideringPreemptions)
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

    constexpr auto currentTime = 5;
    EXPECT_CALL(*timingServiceMock, getTicks()).WillRepeatedly(Return(currentTime));

    constexpr auto task0Flowtime = currentTime - 1 + 4;
    expectNotChangedTaskCall(taskMock0, 1, 4);

    constexpr auto task1Flowtime = currentTime - 3 + 6;
    expectNotChangedTaskCall(taskMock1, 3, 6);

    constexpr auto task2Flowtime = task0Flowtime + currentTime - 2 + 7;
    expectPreemptedTaskCall(taskMock2, 2, 7);

    constexpr auto task3Flowtime = task1Flowtime + currentTime - 4 + 8;
    expectPreemptedTaskCall(taskMock3, 4, 8);

    EXPECT_CALL(*differenceCalculatorMock, calculate(mapping)).WillOnce(Return(diffWithPreemptions));
    ASSERT_EQ(sut.assess(mapping), task0Flowtime + task1Flowtime + task2Flowtime + task3Flowtime);
}

TEST_F(FlowtimeAssessorShould, CalculateFlowtimeConsideringMigrations)
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

    constexpr auto currentTime = 5;
    EXPECT_CALL(*timingServiceMock, getTicks()).WillRepeatedly(Return(currentTime));

    constexpr auto task0Flowtime = currentTime - 1 + 4;
    expectMigratedTaskCall(taskMock0, 1, 4);

    constexpr auto task1Flowtime = currentTime - 3 + 6;
    expectMigratedTaskCall(taskMock1, 3, 6);

    constexpr auto task2Flowtime = task0Flowtime + currentTime - 2 + 7;
    expectNotChangedTaskCall(taskMock2, 2, 7);

    constexpr auto task3Flowtime = task1Flowtime + currentTime - 4 + 8;
    expectNotChangedTaskCall(taskMock3, 4, 8);

    EXPECT_CALL(*differenceCalculatorMock, calculate(mapping)).WillOnce(Return(diffWithMigrations));
    ASSERT_EQ(sut.assess(mapping), task0Flowtime + task1Flowtime + task2Flowtime + task3Flowtime);
}

TEST_F(FlowtimeAssessorShould, CalculateFlowtimeConsideringEverythingAtOnce)
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

    constexpr auto currentTime = 5;
    EXPECT_CALL(*timingServiceMock, getTicks()).WillRepeatedly(Return(currentTime));

    constexpr auto task0Flowtime = currentTime - 2 + 4;
    expectPreemptedTaskCall(taskMock0, 2, 4);

    constexpr auto task1Flowtime = currentTime - 3 + 6;
    expectMigratedTaskCall(taskMock1, 3, 6);

    constexpr auto task2Flowtime = task0Flowtime + currentTime - 5 + 7;
    expectNotChangedTaskCall(taskMock2, 5, 7);

    EXPECT_CALL(*differenceCalculatorMock, calculate(mapping)).WillOnce(Return(diffWithEverything));
    ASSERT_EQ(sut.assess(mapping), task0Flowtime + task1Flowtime + task2Flowtime);
}

} // namespace tests
} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
