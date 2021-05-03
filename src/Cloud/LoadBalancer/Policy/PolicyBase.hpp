#pragma once

#include "Policy.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

class PolicyBase : public Policy
{
  protected:
    PolicyBase(const InfrastructureCPtr &infrastructure, const logger::LoggerPtr &logger);

    const InfrastructureCPtr infrastructure;
    const logger::LoggerPtr logger;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
