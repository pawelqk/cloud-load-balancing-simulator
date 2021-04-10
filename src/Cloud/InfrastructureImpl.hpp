#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Infrastructure.hpp"
#include "Node.hpp"

namespace cloud
{

class InfrastructureImpl : public Infrastructure
{
  public:
    InfrastructureImpl(const std::vector<std::uint32_t> &nodesMips);

    InfrastructureImpl() = delete;
    InfrastructureImpl(const InfrastructureImpl &) = delete;
    InfrastructureImpl(InfrastructureImpl &&) = delete;

    InfrastructureImpl &operator=(const InfrastructureImpl &) = delete;
    InfrastructureImpl &operator=(InfrastructureImpl &&) = delete;

    NodeVec &getNodes() override;
    const NodeVec &getNodes() const override;

    bool isIdle() const override;

    TaskSet advanceProcessing() override;

  private:
    std::vector<Node> prepareNodes(const std::vector<uint32_t> &nodesMips);

    NodeVec nodes;
};

}; // namespace cloud
