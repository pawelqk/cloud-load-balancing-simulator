#include "SimulatedAnnealingBuilder.hpp"

#include "Cloud/LoadBalancer/Mapping/FlowtimeAssessor.hpp"
#include "Cloud/LoadBalancer/Mapping/MakespanAssessor.hpp"

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

PolicyPtr SimulatedAnnealingBuilder::build(const logger::LoggerPtr &logger)
{
    return std::make_unique<SimulatedAnnealing>(infrastructure, parameters, buildAssessor(), logger);
}

std::string SimulatedAnnealingBuilder::toString() const
{
    return "SimulatedAnnealing" + configuration::toString(policyConfiguration);
}

mapping::MappingAssessorPtr SimulatedAnnealingBuilder::buildAssessor()
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

} // namespace simulatedannealing
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
