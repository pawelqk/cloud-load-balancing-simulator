#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Cloud.hpp"
#include "Infrastructure.hpp"
#include "LoadBalancer/Mapping/DifferenceCalculator.hpp"
#include "LoadBalancer/Mapping/DifferenceCalculatorImpl.hpp"
#include "LoadBalancer/Mapping/MappingAssessor.hpp"
#include "LoadBalancer/Policy/Policy.hpp"
#include "Logger/Logger.hpp"
#include "TimingService.hpp"

namespace cloud
{

enum class Policy
{
    ArtificialBeeColony,
    GeneticAlgorithm,
    Random,
    RoundRobin,
    ShortestElapsedTimeFirst,
    ShortestRemainingTimeFirst,
    SimulatedAnnealing,
};

enum class Assessment
{
    Flowtime,
    Makespan
};

std::string toString(const Policy &policy);
std::string toString(const Assessment &assessment);

class CloudBuilder
{
  public:
    CloudBuilder(const Policy &policy, const Assessment &assessment);

    std::unique_ptr<Cloud> build(const std::vector<std::uint32_t> &nodesMips, const TimingServicePtr &timingService,
                                 const logger::LoggerPtr &logger);

  private:
    loadbalancer::policy::PolicyPtr buildPolicy(
        const InfrastructurePtr &infrastructure,
        const loadbalancer::mapping::DifferenceCalculatorPtr &differenceCalculator,
        const TimingServicePtr &timingService, const logger::LoggerPtr &logger);
    loadbalancer::mapping::MappingAssessorPtr buildAssessor(
        const loadbalancer::mapping::DifferenceCalculatorPtr &differenceCalculator,
        const TimingServicePtr &timingService);

    Policy chosenPolicy;
    Assessment chosenAssessment;
};

using CloudBuilderPtr = std::shared_ptr<CloudBuilder>;

} // namespace cloud
