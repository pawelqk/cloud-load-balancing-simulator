#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Infrastructure.hpp"
#include "Logger/Logger.hpp"
#include "Node.hpp"

namespace cloud
{

class InfrastructureImpl : public Infrastructure
{
  public:
    InfrastructureImpl(const std::vector<std::uint32_t> &nodesMips, const logger::LoggerPtr &logger);

    InfrastructureImpl() = delete;
    InfrastructureImpl(const InfrastructureImpl &) = delete;
    InfrastructureImpl(InfrastructureImpl &&) = delete;

    InfrastructureImpl &operator=(const InfrastructureImpl &) = delete;
    InfrastructureImpl &operator=(InfrastructureImpl &&) = delete;

    NodePtrVec &getNodes() override;
    const NodePtrVec &getNodes() const override;

    bool isIdle() const override;

    TaskSet advanceProcessing() override;

  private:
    NodePtrVec prepareNodes(const std::vector<uint32_t> &nodesMips, const logger::LoggerPtr &logger);

    NodePtrVec nodes;
};

}; // namespace cloud
