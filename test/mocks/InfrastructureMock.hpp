#pragma once

#include <memory>
#include <vector>

#include <gmock/gmock.h>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/Task.hpp"

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
    MOCK_METHOD(std::vector<NodeId>, findFeasibleNodeIds, (const TaskPtr &), (const, override));
};

using InfrastructureMockPtr = std::shared_ptr<InfrastructureMock>;

} // namespace mocks
}; // namespace cloud
