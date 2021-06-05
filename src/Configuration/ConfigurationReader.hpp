#pragma once

#include <stdexcept>
#include <vector>

#include "Cloud/LoadBalancer/Policy/PolicyBuilder.hpp"

namespace configuration
{

struct InvalidConfigurationException : std::runtime_error
{
    InvalidConfigurationException(const std::string &msg);
};

struct GeneralConfiguration
{
    std::vector<std::uint_fast64_t> seeds;
    std::vector<double> penaltyFactors;
};

enum class Policy
{
    ArtificialBeeColony,
    FirstComeFirstServe,
    GeneticAlgorithm,
    LongestRemainingTimeFirst,
    Random,
    RoundRobin,
    ShortestElapsedTimeFirst,
    ShortestRemainingTimeFirst,
    SimulatedAnnealing,
};

enum class PolicyConfiguration
{
    Offline,
    Online,
    OnlineWithMigrationsAndPreemptions
};

enum class Assessment
{
    Flowtime,
    Makespan
};

std::string toString(const PolicyConfiguration policyConfiguration);
std::string toString(const Assessment assessment);

class ConfigurationReader
{
  public:
    virtual ~ConfigurationReader();

    virtual GeneralConfiguration readGeneralConfiguration() = 0;
    virtual std::vector<cloud::loadbalancer::policy::PolicyBuilderPtr> readPolicies() = 0;
};

} // namespace configuration
