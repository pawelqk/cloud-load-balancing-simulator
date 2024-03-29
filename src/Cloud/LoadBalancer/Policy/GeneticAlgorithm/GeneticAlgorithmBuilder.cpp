#include "GeneticAlgorithmBuilder.hpp"

#include "Cloud/LoadBalancer/Mapping/OfflineFlowtimeAssessor.hpp"
#include "Cloud/LoadBalancer/Mapping/OfflineMakespanAssessor.hpp"
#include "Cloud/LoadBalancer/Mapping/OnlineFlowtimeAssessor.hpp"
#include "Cloud/LoadBalancer/Mapping/OnlineFlowtimeAssessorWithMigrationsAndPreemptions.hpp"
#include "Cloud/LoadBalancer/Mapping/OnlineMakespanAssessor.hpp"
#include "Cloud/LoadBalancer/Mapping/OnlineMakespanAssessorWithMigrationsAndPreemptions.hpp"
#include "Configuration/ConfigurationReader.hpp"
#include "OfflineGeneticAlgorithm.hpp"
#include "OnlineGeneticAlgorithm.hpp"
#include "OnlineGeneticAlgorithmWithMigrationsAndPreemptions.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

GeneticAlgorithmBuilder::GeneticAlgorithmBuilder(const configuration::PolicyConfiguration &policyConfiguration,
                                                 const configuration::Assessment assessment,
                                                 const Parameters &parameters)
    : PolicyBuilderBase(policyConfiguration), assessment(assessment), parameters(parameters)
{
}

PolicyBuilderPtr GeneticAlgorithmBuilder::clone()
{
    return std::make_shared<GeneticAlgorithmBuilder>(policyConfiguration, assessment, parameters);
}

PolicyPtr GeneticAlgorithmBuilder::build(const logger::LoggerPtr &logger)
{
    using configuration::PolicyConfiguration;
    switch (policyConfiguration)
    {
    case PolicyConfiguration::Offline:
        return std::make_unique<OfflineGeneticAlgorithm>(infrastructure, parameters, buildOfflineAssessor(), *instance,
                                                         logger, randomNumberGenerator, penaltyFactor);
    case PolicyConfiguration::Online:
        return std::make_unique<OnlineGeneticAlgorithm>(infrastructure, parameters, buildOnlineAssessor(), logger,
                                                        randomNumberGenerator);
    case PolicyConfiguration::OnlineWithMigrationsAndPreemptions:
        return std::make_unique<OnlineGeneticAlgorithmWithMigrationsAndPreemptions>(
            infrastructure, parameters, buildAssessor(), logger, randomNumberGenerator);
    }

    return nullptr;
}

std::string GeneticAlgorithmBuilder::toString() const
{
    return "GeneticAlgorithm-" + configuration::toString(policyConfiguration) + "-" +
           configuration::toString(assessment) + "-" +
           ::cloud::loadbalancer::policy::geneticalgorithm::toString(parameters);
}

std::shared_ptr<mapping::MappingAssessor> GeneticAlgorithmBuilder::buildAssessor()
{
    switch (assessment)
    {
    case configuration::Assessment::Makespan:
        return std::make_shared<mapping::OnlineMakespanAssessorWithMigrationsAndPreemptions>(differenceCalculator);
    case configuration::Assessment::Flowtime:
        return std::make_shared<mapping::OnlineFlowtimeAssessorWithMigrationsAndPreemptions>(differenceCalculator,
                                                                                             timingService);
    }

    return {};
}

std::shared_ptr<mapping::MappingAssessor> GeneticAlgorithmBuilder::buildOnlineAssessor()
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

std::shared_ptr<mapping::MappingAssessor> GeneticAlgorithmBuilder::buildOfflineAssessor()
{
    switch (assessment)
    {
    case configuration::Assessment::Makespan:
        return std::make_shared<mapping::OfflineMakespanAssessor>();
    case configuration::Assessment::Flowtime:
        return std::make_shared<mapping::OfflineFlowtimeAssessor>();
    }

    return {};
}

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
