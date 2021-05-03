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
    std::uint_fast64_t seed;
    double penaltyFactor;
};

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

enum class PolicyConfiguration
{
    Offline,
    Online,
    OnlineWithMigrations,
    OnlineWithMigrationsAndPreemptions
};

enum class Assessment
{
    Flowtime,
    Makespan
};

std::string toString(const PolicyConfiguration policyConfiguration);

class ConfigurationReader
{
  public:
    virtual ~ConfigurationReader();

    virtual GeneralConfiguration readGeneralConfiguration() = 0;
    virtual std::vector<cloud::loadbalancer::policy::PolicyBuilderPtr> readPolicies() = 0;
};

} // namespace configuration
