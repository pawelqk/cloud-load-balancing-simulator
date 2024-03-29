#include "ArtificialBeeColonyBuilder.hpp"

#include "Cloud/LoadBalancer/Mapping/OfflineFlowtimeAssessor.hpp"
#include "Cloud/LoadBalancer/Mapping/OfflineMakespanAssessor.hpp"
#include "Cloud/LoadBalancer/Mapping/OnlineFlowtimeAssessor.hpp"
#include "Cloud/LoadBalancer/Mapping/OnlineFlowtimeAssessorWithMigrationsAndPreemptions.hpp"
#include "Cloud/LoadBalancer/Mapping/OnlineMakespanAssessor.hpp"
#include "Cloud/LoadBalancer/Mapping/OnlineMakespanAssessorWithMigrationsAndPreemptions.hpp"
#include "Configuration/ConfigurationReader.hpp"
#include "OfflineArtificialBeeColony.hpp"
#include "OnlineArtificialBeeColony.hpp"
#include "OnlineArtificialBeeColonyWithMigrationsAndPreemptions.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace artificialbeecolony
{

ArtificialBeeColonyBuilder::ArtificialBeeColonyBuilder(const configuration::PolicyConfiguration &policyConfiguration,
                                                       const configuration::Assessment assessment,
                                                       const Parameters &parameters)
    : PolicyBuilderBase(policyConfiguration), assessment(assessment), parameters(parameters)
{
}

PolicyBuilderPtr ArtificialBeeColonyBuilder::clone()
{
    return std::make_shared<ArtificialBeeColonyBuilder>(policyConfiguration, assessment, parameters);
}

PolicyPtr ArtificialBeeColonyBuilder::build(const logger::LoggerPtr &logger)
{
    using configuration::PolicyConfiguration;
    switch (policyConfiguration)
    {
    case PolicyConfiguration::Offline:
        return std::make_unique<OfflineArtificialBeeColony>(infrastructure, parameters, buildOfflineAssessor(),
                                                            randomNumberGenerator, *instance, logger, penaltyFactor);
    case PolicyConfiguration::Online:
        return std::make_unique<OnlineArtificialBeeColony>(infrastructure, parameters, buildOnlineAssessor(),
                                                           randomNumberGenerator, logger);
    case PolicyConfiguration::OnlineWithMigrationsAndPreemptions:
        return std::make_unique<OnlineArtificialBeeColonyWithMigrationsAndPreemptions>(
            infrastructure, parameters, buildAssessor(), randomNumberGenerator, logger);
    }

    return nullptr;
}

std::string ArtificialBeeColonyBuilder::toString() const
{
    return "ArtificialBeeColony-" + configuration::toString(policyConfiguration) + "-" +
           configuration::toString(assessment) + "-" +
           ::cloud::loadbalancer::policy::artificialbeecolony::toString(parameters);
}

mapping::MappingAssessorPtr ArtificialBeeColonyBuilder::buildAssessor()
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

mapping::MappingAssessorPtr ArtificialBeeColonyBuilder::buildOnlineAssessor()
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

mapping::MappingAssessorPtr ArtificialBeeColonyBuilder::buildOfflineAssessor()
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

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
