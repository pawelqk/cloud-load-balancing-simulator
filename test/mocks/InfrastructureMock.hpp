#pragma once

#include <memory>

#include <gmock/gmock.h>

#include "Cloud/Infrastructure.hpp"

namespace cloud
{
namespace mocks
{

class InfrastructureMock : public testing::StrictMock<Infrastructure>
{
  public:
    MOCK_METHOD(NodePtrVec &, getNodes, (), (override));
    MOCK_METHOD(const NodePtrVec &, getNodes, (), (const, override));

    MOCK_METHOD(bool, isIdle, (), (const, override));

    MOCK_METHOD(TaskPtrVec, advanceProcessing, (), (override));
};

using InfrastructureMockPtr = std::shared_ptr<InfrastructureMock>;

} // namespace mocks
}; // namespace cloud
