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
    NodeImpl(NodeImpl &&) = delete;
    NodeImpl &operator=(const NodeImpl &) = delete;
    NodeImpl &operator=(NodeImpl &&) = delete;

    void assign(const TaskPtr &task) override;
    void work() override;
    TaskPtr extractTask() override;

    bool canTaskFit(const TaskPtr &task) const override;
    bool isIdle() const override;

    TaskPtr getTask() const override;
    NodeId getId() const override;

    std::string toString() const override;

  private:
    const NodeId id;
    const std::uint32_t mips;

    TaskPtr task;
    const logger::LoggerPtr logger;
};

} // namespace cloud
