#include "PolicyBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

PolicyBase::PolicyBase(const InfrastructureCPtr &infrastructure, const logger::LoggerPtr logger)
    : infrastructure(infrastructure), logger(logger)
{
}

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
