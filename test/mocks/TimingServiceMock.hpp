#pragma once

#include <cstdint>
#include <memory>

#include <gmock/gmock.h>

#include "Cloud/TimingService.hpp"

namespace cloud
{
namespace mocks
{

class TimingServiceMock : public testing::StrictMock<TimingService>
{
  public:
    MOCK_METHOD(void, tick, (), (override));
    MOCK_METHOD(void, addFlowtime, (const std::uint32_t), (override));

    MOCK_METHOD(std::uint32_t, getTicks, (), (const, override));
    MOCK_METHOD(std::uint32_t, getTotalFlowtime, (), (const, override));
};

using TimingServiceMockPtr = std::shared_ptr<TimingServiceMock>;

} // namespace mocks
} // namespace cloud
