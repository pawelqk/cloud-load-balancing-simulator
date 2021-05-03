#pragma once

#include <memory>
#include <optional>

#include <gmock/gmock.h>

#include "Cloud/Task.hpp"

namespace cloud
{
namespace mocks
{

class TaskMock : public testing::StrictMock<Task>
{
  public:
    TaskMock(const std::uint32_t id) : id(id)
    {
    }

    MOCK_METHOD(void, work, (), (override));
    MOCK_METHOD(bool, isDone, (), (const, override));
    MOCK_METHOD(void, performPreemption, (), (override));
    MOCK_METHOD(void, performMigration, (), (override));

    inline std::uint32_t getId() const override
    {
        return id;
    }
    MOCK_METHOD(std::uint32_t, getMips, (), (const, override));
    MOCK_METHOD(std::uint32_t, getInitialLength, (), (const, override));
    MOCK_METHOD(std::uint32_t, getArrivalTime, (), (const, override));
    MOCK_METHOD(std::uint32_t, getElapsedTime, (), (const, override));
    MOCK_METHOD(std::uint32_t, estimateTimeLeft, (), (const, override));
    MOCK_METHOD(std::uint32_t, estimateTimeLeftAfterMigration, (), (const, override));
    MOCK_METHOD(std::uint32_t, estimateTimeLeftAfterPreemption, (), (const, override));

  private:
    const std::uint32_t id;
};

using TaskMockPtr = std::shared_ptr<TaskMock>;

} // namespace mocks
}; // namespace cloud
