#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Node.hpp"

namespace cloud
{

class Infrastructure
{
  public:
    Infrastructure(const std::vector<std::uint32_t> &nodesMips);

    Infrastructure() = delete;
    Infrastructure(const Infrastructure &) = delete;
    Infrastructure(Infrastructure &&) = delete;

    Infrastructure &operator=(const Infrastructure &) = delete;
    Infrastructure &operator=(Infrastructure &&) = delete;

    NodeVec &getNodes();
    const NodeVec &getNodes() const;

    bool isIdle() const;

    TaskSet advanceProcessing();

  private:
    std::vector<Node> prepareNodes(const std::vector<uint32_t> &nodesMips);

    NodeVec nodes;
};

using InfrastructurePtr = std::shared_ptr<Infrastructure>;
using InfrastructureCPtr = std::shared_ptr<const Infrastructure>;

}; // namespace cloud
