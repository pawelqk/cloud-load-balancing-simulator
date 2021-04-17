#pragma once

#include <memory>

#include "Node.hpp"

namespace cloud
{

class Infrastructure
{
  public:
    virtual NodePtrVec &getNodes() = 0;
    virtual const NodePtrVec &getNodes() const = 0;

    virtual bool isIdle() const = 0;

    virtual TaskPtrVec advanceProcessing() = 0;
};

using InfrastructurePtr = std::shared_ptr<Infrastructure>;
using InfrastructureCPtr = std::shared_ptr<const Infrastructure>;

}; // namespace cloud
