#pragma once

#include <memory>

#include "Node.hpp"

namespace cloud
{

class Infrastructure
{
  public:
    Infrastructure(const NodeVec &nodes);

    Infrastructure() = delete;
    Infrastructure(const Infrastructure &) = delete;
    Infrastructure(Infrastructure &&) = delete;

    Infrastructure &operator=(const Infrastructure &) = delete;
    Infrastructure &operator=(Infrastructure &&) = delete;

    NodeVec &getNodes();
    const NodeVec &getNodes() const;

  private:
    NodeVec nodes;
};

using InfrastructurePtr = std::shared_ptr<Infrastructure>;
using InfrastructureCPtr = std::shared_ptr<const Infrastructure>;

}; // namespace cloud
