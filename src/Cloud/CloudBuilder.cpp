#include "CloudBuilder.hpp"
#include "Cloud/LoadBalancer/MakespanAssessor.hpp"
#include "InfrastructureImpl.hpp"
#include "LoadBalancer/LoadBalancerImpl.hpp"
#include "LoadBalancer/Policy/RoundRobin.hpp"
#include "LoadBalancer/Policy/SimulatedAnnealing.hpp"

namespace cloud
{

std::unique_ptr<Cloud> CloudBuilder::build(const std::vector<std::uint32_t> &nodesMips)
{
    const auto infrastructure = std::make_shared<InfrastructureImpl>(nodesMips);
    loadbalancer::policy::SimulatedAnnealing::Parameters params{
        0.997, 1000, 0.00001, 1000, std::make_unique<loadbalancer::MakespanAssessor>(infrastructure)};
    return std::make_unique<Cloud>(
        std::make_unique<loadbalancer::LoadBalancerImpl>(
            std::make_unique<loadbalancer::policy::SimulatedAnnealing>(infrastructure, std::move(params)),
            infrastructure),
        infrastructure);
}

} // namespace cloud
