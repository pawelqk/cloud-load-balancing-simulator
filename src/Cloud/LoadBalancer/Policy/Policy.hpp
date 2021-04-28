#pragma once

#include <list>
#include <map>
#include <memory>
#include <optional>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/Node.hpp"
#include "Cloud/Task.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

using NodeToTaskMapping = std::map<NodeId, TaskPtrList>;

std::string toString(const NodeToTaskMapping &mapping);

class Policy
{
  public:
    virtual ~Policy();

    virtual NodeToTaskMapping buildNodeToTaskMapping(const TaskPtrVec &tasks) = 0;

    virtual std::string toString() const = 0;
};

using PolicyPtr = std::unique_ptr<Policy>;

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
