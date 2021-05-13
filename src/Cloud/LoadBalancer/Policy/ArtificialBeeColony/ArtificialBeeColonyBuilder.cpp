#include "ArtificialBeeColonyBuilder.hpp"

#include "Cloud/LoadBalancer/Mapping/FlowtimeAssessor.hpp"
#include "Cloud/LoadBalancer/Mapping/MakespanAssessor.hpp"
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
        return std::make_unique<OfflineArtificialBeeColony>(infrastructure, parameters, buildAssessor(), *instance,
                                                            logger);
    case PolicyConfiguration::Online:
        return std::make_unique<OnlineArtificialBeeColony>(infrastructure, parameters, buildAssessor(), logger);
    case PolicyConfiguration::OnlineWithMigrationsAndPreemptions:
        return std::make_unique<OnlineArtificialBeeColonyWithMigrationsAndPreemptions>(infrastructure, parameters,
                                                                                       buildAssessor(), logger);
    }

    return nullptr;
}

std::string ArtificialBeeColonyBuilder::toString() const
{
    return "ArtificialBeeColony" + configuration::toString(policyConfiguration);
}

mapping::MappingAssessorPtr ArtificialBeeColonyBuilder::buildAssessor()
{
    switch (assessment)
    {
    case configuration::Assessment::Makespan:
        return std::make_unique<mapping::MakespanAssessor>(differenceCalculator);
    case configuration::Assessment::Flowtime:
        return std::make_unique<mapping::FlowtimeAssessor>(differenceCalculator, timingService);
    }

    return {};
}

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
