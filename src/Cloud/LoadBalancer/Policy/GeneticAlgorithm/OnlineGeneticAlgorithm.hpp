#pragma once

#include <functional>
#include <list>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"
#include "Cloud/Task.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

class OnlineGeneticAlgorithm : public PolicyBase
{
  public:
    struct Parameters
    {
        mapping::MappingAssessorPtr solutionAssessor;
    };

    OnlineGeneticAlgorithm(const InfrastructureCPtr &infrastructure, Parameters &&parameters,
                           const logger::LoggerPtr &logger);

    NodeToTaskMapping buildNodeToTaskMapping(const TaskPtrVec &tasks) override;

  private:
    const Parameters parameters;

    const logger::LoggerPtr logger;
};

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
