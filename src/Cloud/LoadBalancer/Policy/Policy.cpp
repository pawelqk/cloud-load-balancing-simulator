#include "Policy.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

Policy::Policy(const InfrastructureCPtr &infrastructure) : infrastructure(infrastructure)
{
}

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
