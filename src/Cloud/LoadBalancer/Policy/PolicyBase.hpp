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
    PolicyBase(const InfrastructureCPtr &infrastructure);

    const InfrastructureCPtr infrastructure;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
