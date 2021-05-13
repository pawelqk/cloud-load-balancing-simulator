#include "GeneticAlgorithmBuilder.hpp"

#include "Cloud/LoadBalancer/Mapping/FlowtimeAssessor.hpp"
#include "Cloud/LoadBalancer/Mapping/MakespanAssessor.hpp"
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
        return std::make_unique<OfflineGeneticAlgorithm>(infrastructure, parameters, buildAssessor(), *instance,
                                                         logger);
    case PolicyConfiguration::Online:
        return std::make_unique<OnlineGeneticAlgorithm>(infrastructure, parameters, buildAssessor(), logger);
    case PolicyConfiguration::OnlineWithMigrationsAndPreemptions:
        return std::make_unique<OnlineGeneticAlgorithmWithMigrationsAndPreemptions>(infrastructure, parameters,
                                                                                    buildAssessor(), logger);
    }

    return nullptr;
}

std::string GeneticAlgorithmBuilder::toString() const
{
    return "GeneticAlgorithm" + configuration::toString(policyConfiguration);
}

mapping::MappingAssessorPtr GeneticAlgorithmBuilder::buildAssessor()
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

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
