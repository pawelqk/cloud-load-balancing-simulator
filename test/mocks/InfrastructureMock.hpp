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
    MOCK_METHOD(NodeVec &, getNodes, ());
    MOCK_METHOD(const NodeVec &, getNodes, (), (const));

    MOCK_METHOD(bool, isIdle, (), (const));

    MOCK_METHOD(TaskSet, advanceProcessing, ());
};

using InfrastructureMockPtr = std::shared_ptr<InfrastructureMock>;

} // namespace mocks
}; // namespace cloud
