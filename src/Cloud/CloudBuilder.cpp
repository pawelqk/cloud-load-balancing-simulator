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

CloudBuilder::CloudBuilder(const Policy &policy, const Assessment &assessment)
    : chosenPolicy(policy), chosenAssessment(assessment)
{
}

std::unique_ptr<Cloud> CloudBuilder::build(const std::vector<std::uint32_t> &nodesMips,
                                           const TimingServicePtr &timingService, const logger::LoggerPtr &logger)
{
    const auto infrastructure = std::make_shared<InfrastructureImpl>(nodesMips, logger);
    const auto differenceCalculator = std::make_shared<loadbalancer::mapping::DifferenceCalculatorImpl>(infrastructure);
    return std::make_unique<Cloud>(std::make_unique<loadbalancer::LoadBalancerImpl>(
                                       buildPolicy(infrastructure, differenceCalculator, timingService, logger),
                                       infrastructure, differenceCalculator, logger),
                                   infrastructure, timingService, logger);
}

loadbalancer::policy::PolicyPtr CloudBuilder::buildPolicy(
    const InfrastructurePtr &infrastructure, const loadbalancer::mapping::DifferenceCalculatorPtr &differenceCalculator,
    const TimingServicePtr &timingService, const logger::LoggerPtr &logger)
{
    switch (chosenPolicy)
    {
    case Policy::Random:
        return std::make_unique<loadbalancer::policy::Random>(infrastructure, logger);
    case Policy::RoundRobin:
        return std::make_unique<loadbalancer::policy::RoundRobin>(infrastructure);
    case Policy::ShortestRemainingTimeFirst:
        return std::make_unique<loadbalancer::policy::ShortestRemainingTimeFirst>(infrastructure);
    case Policy::SimulatedAnnealing: {
        loadbalancer::policy::SimulatedAnnealing::Parameters params{0.997, 1000, 0.00001, 1000,
                                                                    buildAssessor(differenceCalculator, timingService)};
        return std::make_unique<loadbalancer::policy::SimulatedAnnealing>(infrastructure, std::move(params), logger);
    }
    default:
        throw std::runtime_error("Unimplemented policy!");
    }
}

loadbalancer::mapping::MappingAssessorPtr CloudBuilder::buildAssessor(
    const loadbalancer::mapping::DifferenceCalculatorPtr &differenceCalculator, const TimingServicePtr &timingService)
{
    switch (chosenAssessment)
    {
    case Assessment::Flowtime:
        return std::make_unique<loadbalancer::mapping::FlowtimeAssessor>(differenceCalculator, timingService);
    case Assessment::Makespan:
        return std::make_unique<loadbalancer::mapping::MakespanAssessor>(differenceCalculator);
    }

    return {};
}

} // namespace cloud
