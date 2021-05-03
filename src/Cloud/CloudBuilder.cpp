#include "CloudBuilder.hpp"

#include "InfrastructureImpl.hpp"
#include "LoadBalancer/LoadBalancerImpl.hpp"

namespace cloud
{

CloudBuilder::CloudBuilder(const configuration::NodeDataVec &nodesData, const TimingServicePtr &timingService,
                           const cloud::loadbalancer::policy::PolicyBuilderPtr &policyBuilder,
                           const double penaltyFactor, const logger::LoggerPtr &logger)
    : nodesData(nodesData), timingService(timingService), policyBuilder(policyBuilder), penaltyFactor(penaltyFactor),
      logger(logger)
{
}

std::unique_ptr<Cloud> CloudBuilder::build()
{
    const auto infrastructure = std::make_shared<InfrastructureImpl>(nodesData, logger);
    policyBuilder->setInfrastructure(infrastructure);

    const auto differenceCalculator = std::make_shared<loadbalancer::mapping::DifferenceCalculatorImpl>(infrastructure);
    policyBuilder->setDifferenceCalculator(differenceCalculator);

    return std::make_unique<Cloud>(std::make_unique<loadbalancer::LoadBalancerImpl>(
                                       policyBuilder->build(logger), infrastructure, differenceCalculator, logger),
                                   infrastructure, timingService, penaltyFactor, logger);
}

} // namespace cloud
