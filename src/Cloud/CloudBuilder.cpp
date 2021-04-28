#include "CloudBuilder.hpp"

#include <stdexcept>

#include "InfrastructureImpl.hpp"
#include "LoadBalancer/LoadBalancerImpl.hpp"
#include "LoadBalancer/Mapping/FlowtimeAssessor.hpp"
#include "LoadBalancer/Mapping/MakespanAssessor.hpp"
#include "LoadBalancer/Policy/Policy.hpp"
#include "LoadBalancer/Policy/Random.hpp"
#include "LoadBalancer/Policy/RoundRobin.hpp"
#include "LoadBalancer/Policy/ShortestRemainingTimeFirst.hpp"
#include "LoadBalancer/Policy/SimulatedAnnealing.hpp"

namespace cloud
{

std::string toString(const Policy &policy)
{
#define CASE(policy)                                                                                                   \
    case (Policy::policy):                                                                                             \
        return #policy;

    switch (policy)
    {
        CASE(ArtificialBeeColony)
        CASE(GeneticAlgorithm)
        CASE(Random)
        CASE(RoundRobin)
        CASE(ShortestElapsedTimeFirst)
        CASE(ShortestRemainingTimeFirst)
        CASE(SimulatedAnnealing)
    }

#undef CASE

    return "";
}

std::string toString(const Assessment &assessment)
{

#define CASE(assessment)                                                                                               \
    case (Assessment::assessment):                                                                                     \
        return #assessment;

    switch (assessment)
    {
        CASE(Flowtime)
        CASE(Makespan)
    }

#undef CASE

    return "";
}

CloudBuilder::CloudBuilder(const std::vector<std::uint32_t> &nodesMips, const TimingServicePtr &timingService,
                           const cloud::loadbalancer::policy::builders::PolicyBuilderPtr &policyBuilder,
                           const logger::LoggerPtr &logger)
    : nodesMips(nodesMips), timingService(timingService), policyBuilder(policyBuilder), logger(logger)
{
}

std::unique_ptr<Cloud> CloudBuilder::build()
{
    const auto infrastructure = std::make_shared<InfrastructureImpl>(nodesMips, logger);
    policyBuilder->setInfrastructure(infrastructure);

    const auto differenceCalculator = std::make_shared<loadbalancer::mapping::DifferenceCalculatorImpl>(infrastructure);
    return std::make_unique<Cloud>(std::make_unique<loadbalancer::LoadBalancerImpl>(
                                       policyBuilder->build(logger), infrastructure, differenceCalculator, logger),
                                   infrastructure, timingService, logger);
}

} // namespace cloud
