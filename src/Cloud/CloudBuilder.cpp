#include "CloudBuilder.hpp"

#include <stdexcept>

#include "InfrastructureImpl.hpp"
#include "LoadBalancer/LoadBalancerImpl.hpp"

namespace cloud
{

CloudBuilder::CloudBuilder(const std::vector<std::uint32_t> &nodesMips, const TimingServicePtr &timingService,
                           const cloud::loadbalancer::policy::PolicyBuilderPtr &policyBuilder,
                           const logger::LoggerPtr &logger)
    : nodesMips(nodesMips), timingService(timingService), policyBuilder(policyBuilder), logger(logger)
{
}

std::unique_ptr<Cloud> CloudBuilder::build()
{
    const auto infrastructure = std::make_shared<InfrastructureImpl>(nodesMips, logger);
    policyBuilder->setInfrastructure(infrastructure);

    const auto differenceCalculator = std::make_shared<loadbalancer::mapping::DifferenceCalculatorImpl>(infrastructure);
    policyBuilder->setDifferenceCalculator(differenceCalculator);

    return std::make_unique<Cloud>(std::make_unique<loadbalancer::LoadBalancerImpl>(
                                       policyBuilder->build(logger), infrastructure, differenceCalculator, logger),
                                   infrastructure, timingService, logger);
}

} // namespace cloud
