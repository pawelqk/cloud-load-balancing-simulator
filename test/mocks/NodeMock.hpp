#pragma once

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

    MOCK_METHOD(void, assign, (const Task &), (override));
    MOCK_METHOD(void, work, (), (override));
    MOCK_METHOD(Task, extractTask, (), (override));

    MOCK_METHOD(bool, canTaskFit, (const Task &), (const, override));
    MOCK_METHOD(bool, isIdle, (), (const, override));

    MOCK_METHOD(std::optional<Task>, getTask, (), (const, override));

    inline NodeId getId() const override
    {
        return id;
    }

    MOCK_METHOD(std::string, toString, (), (const, override));

  private:
    const NodeId id;
};

using NodeMockPtr = std::shared_ptr<Node>;

} // namespace mocks
}; // namespace cloud
