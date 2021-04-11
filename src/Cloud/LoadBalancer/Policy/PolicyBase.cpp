#include "PolicyBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

PolicyBase::PolicyBase(const InfrastructureCPtr &infrastructure) : infrastructure(infrastructure)
{
}

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
