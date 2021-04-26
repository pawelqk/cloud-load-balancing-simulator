#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include <gmock/gmock.h>

#include "Cloud/Node.hpp"

namespace cloud
{
namespace mocks
{

class NodeMock : public testing::StrictMock<Node>
{
  public:
    NodeMock(const NodeId id) : id(id)
    {
    }

    MOCK_METHOD(void, assign, (const TaskPtr &), (override));
    MOCK_METHOD(void, work, (), (override));
    MOCK_METHOD(TaskPtr, extractTask, (), (override));

    MOCK_METHOD(bool, canTaskFit, (const TaskPtr &), (const, override));
    MOCK_METHOD(bool, isIdle, (), (const, override));

    MOCK_METHOD(TaskPtr, getTask, (), (const, override));

    inline NodeId getId() const override
    {
        return id;
    }

    MOCK_METHOD(std::uint32_t, getMips, (), (const, override));

    inline std::string toString() const override
    {
        return "";
    }

  private:
    const NodeId id;
};

using NodeMockPtr = std::shared_ptr<NodeMock>;

} // namespace mocks
}; // namespace cloud
