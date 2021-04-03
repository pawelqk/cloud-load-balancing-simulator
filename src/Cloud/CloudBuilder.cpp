#include "CloudBuilder.hpp"
#include "LoadBalancer/LoadBalancerImpl.hpp"
#include "LoadBalancer/Strategy/Random.hpp"

namespace cloud
{

std::unique_ptr<Cloud> CloudBuilder::build(const std::vector<std::uint32_t> &nodesMips)
{
    const auto infrastructure = std::make_shared<Infrastructure>(nodesMips);
    return std::make_unique<Cloud>(
        std::make_unique<loadbalancer::LoadBalancerImpl>(
            std::make_unique<loadbalancer::strategy::Random>(infrastructure), infrastructure),
        infrastructure);
}

} // namespace cloud
