#pragma once

#include <cstdint>
#include <optional>
#include <set>
#include <vector>

#include "Logger/Logger.hpp"
#include "Node.hpp"
#include "Task.hpp"

namespace cloud
{

using NodeId = std::uint32_t;

class NodeImpl : public Node
{
  public:
    explicit NodeImpl(const NodeId id, const std::uint32_t mips, const logger::LoggerPtr &logger);
    NodeImpl() = delete;
    NodeImpl(const NodeImpl &) = delete;
    NodeImpl(NodeImpl &&) = default;
    NodeImpl &operator=(const NodeImpl &) = delete;
    NodeImpl &operator=(NodeImpl &&) = delete;

    void assign(const Task &task) override;
    void work() override;
    Task extractTask() override;

    bool canTaskFit(const Task &task) const override;
    bool isIdle() const override;

    std::optional<Task> getTask() const override;
    NodeId getId() const override;

    std::string toString() const override;

  private:
    const NodeId id;
    const std::uint32_t mips;

    std::optional<Task> task;
    const logger::LoggerPtr logger;
};

} // namespace cloud
