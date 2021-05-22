#include "CloudBuilder.hpp"

#include "InfrastructureImpl.hpp"
#include "LoadBalancer/LoadBalancerImpl.hpp"

namespace cloud
{

CloudBuilder::CloudBuilder(const configuration::NodeDataVec &nodesData, const TimingServicePtr &timingService,
                           const cloud::loadbalancer::policy::PolicyBuilderPtr &policyBuilder,
                           const std::uint_fast64_t seed, const double penaltyFactor, const logger::LoggerPtr &logger)
    : nodesData(nodesData), timingService(timingService), policyBuilder(policyBuilder), seed(seed),
      penaltyFactor(penaltyFactor), logger(logger)
{
}

std::unique_ptr<Cloud> CloudBuilder::build()
{
    const auto randomNumberGenerator = std::make_shared<utility::RandomNumberGenerator>(seed);
    policyBuilder->setRandomNumberGenerator(randomNumberGenerator);

    const auto infrastructure = std::make_shared<InfrastructureImpl>(nodesData, logger);
    policyBuilder->setInfrastructure(infrastructure);

    const auto differenceCalculator = std::make_shared<loadbalancer::mapping::DifferenceCalculatorImpl>(infrastructure);
    policyBuilder->setDifferenceCalculator(differenceCalculator);

    return std::make_unique<Cloud>(std::make_unique<loadbalancer::LoadBalancerImpl>(
                                       policyBuilder->build(logger), infrastructure, differenceCalculator, logger),
                                   infrastructure, timingService, penaltyFactor, logger);
}

} // namespace cloud
