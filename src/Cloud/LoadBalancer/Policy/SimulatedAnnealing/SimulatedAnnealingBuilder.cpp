#include "SimulatedAnnealingBuilder.hpp"

#include "Cloud/LoadBalancer/Mapping/OfflineFlowtimeAssessor.hpp"
#include "Cloud/LoadBalancer/Mapping/OfflineMakespanAssessor.hpp"
#include "Cloud/LoadBalancer/Mapping/OnlineFlowtimeAssessor.hpp"
#include "Cloud/LoadBalancer/Mapping/OnlineFlowtimeAssessorWithMigrationsAndPreemptions.hpp"
#include "Cloud/LoadBalancer/Mapping/OnlineMakespanAssessor.hpp"
#include "Cloud/LoadBalancer/Mapping/OnlineMakespanAssessorWithMigrationsAndPreemptions.hpp"
#include "Configuration/ConfigurationReader.hpp"
#include "OfflineSimulatedAnnealing.hpp"
#include "OnlineSimulatedAnnealing.hpp"
#include "OnlineSimulatedAnnealingWithMigrationsAndPreemptions.hpp"
#include <memory>

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace simulatedannealing
{

SimulatedAnnealingBuilder::SimulatedAnnealingBuilder(const configuration::PolicyConfiguration &policyConfiguration,
                                                     const configuration::Assessment assessment,
                                                     const Parameters &parameters)
    : PolicyBuilderBase(policyConfiguration), assessment(assessment), parameters(parameters)
{
}

PolicyBuilderPtr SimulatedAnnealingBuilder::clone()
{
    return std::make_shared<SimulatedAnnealingBuilder>(policyConfiguration, assessment, parameters);
}

PolicyPtr SimulatedAnnealingBuilder::build(const logger::LoggerPtr &logger)
{
    using configuration::PolicyConfiguration;
    switch (policyConfiguration)
    {
    case PolicyConfiguration::Offline:
        return std::make_unique<OfflineSimulatedAnnealing>(infrastructure, parameters, buildOfflineAssessor(),
                                                           randomNumberGenerator, *instance, logger, penaltyFactor);
    case PolicyConfiguration::Online:
        return std::make_unique<OnlineSimulatedAnnealing>(infrastructure, parameters, buildOnlineAssessor(),
                                                          randomNumberGenerator, logger);
    case PolicyConfiguration::OnlineWithMigrationsAndPreemptions:
        return std::make_unique<OnlineSimulatedAnnealingWithMigrationsAndPreemptions>(
            infrastructure, parameters, buildAssessor(), randomNumberGenerator, logger);
    }

    return nullptr;
}

std::string SimulatedAnnealingBuilder::toString() const
{
    return "SimulatedAnnealing-" + configuration::toString(policyConfiguration) + "-" +
           configuration::toString(assessment) + "-" +
           ::cloud::loadbalancer::policy::simulatedannealing::toString(parameters);
}

mapping::MappingAssessorPtr SimulatedAnnealingBuilder::buildAssessor()
{
    switch (assessment)
    {
    case configuration::Assessment::Makespan:
        return std::make_unique<mapping::OnlineMakespanAssessorWithMigrationsAndPreemptions>(differenceCalculator);
    case configuration::Assessment::Flowtime:
        return std::make_unique<mapping::OnlineFlowtimeAssessorWithMigrationsAndPreemptions>(differenceCalculator,
                                                                                             timingService);
    }

    return {};
}

mapping::MappingAssessorPtr SimulatedAnnealingBuilder::buildOnlineAssessor()
{
    switch (assessment)
    {
    case configuration::Assessment::Makespan:
        return std::make_unique<mapping::OnlineMakespanAssessor>(infrastructure);
    case configuration::Assessment::Flowtime:
        return std::make_unique<mapping::OnlineFlowtimeAssessor>(infrastructure, timingService);
    }

    return {};
}

mapping::MappingAssessorPtr SimulatedAnnealingBuilder::buildOfflineAssessor()
{
    switch (assessment)
    {
    case configuration::Assessment::Makespan:
        return std::make_unique<mapping::OfflineMakespanAssessor>();
    case configuration::Assessment::Flowtime:
        return std::make_unique<mapping::OfflineFlowtimeAssessor>();
    }

    return {};
}

} // namespace simulatedannealing
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
