#pragma once

#include <list>
#include <map>
#include <memory>
#include <optional>
#include <ostream>
#include <stdexcept>

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

void PrintTo(const NodeToTaskMapping &mapping, std::ostream *os);

struct PolicyException : std::runtime_error
{
    PolicyException(const std::string &msg);
};

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
