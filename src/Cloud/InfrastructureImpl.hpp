#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Configuration/Instance.hpp"
#include "Infrastructure.hpp"
#include "Logger/Logger.hpp"
#include "Node.hpp"

namespace cloud
{

class InfrastructureImpl : public Infrastructure
{
  public:
    InfrastructureImpl(const configuration::NodeDataVec &nodesData, const logger::LoggerPtr &logger);

    InfrastructureImpl() = delete;
    InfrastructureImpl(const InfrastructureImpl &) = delete;
    InfrastructureImpl(InfrastructureImpl &&) = delete;

    InfrastructureImpl &operator=(const InfrastructureImpl &) = delete;
    InfrastructureImpl &operator=(InfrastructureImpl &&) = delete;

    NodePtrVec &getNodes() override;
    const NodePtrVec &getNodes() const override;

    bool isIdle() const override;

    TaskPtrVec advanceProcessing() override;

  private:
    void prepareNodes(const configuration::NodeDataVec &nodesData);
    std::string toString() const;

    const logger::LoggerPtr logger;
    NodePtrVec nodes;
};

}; // namespace cloud
