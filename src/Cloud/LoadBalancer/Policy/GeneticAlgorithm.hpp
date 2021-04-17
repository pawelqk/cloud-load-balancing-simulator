#pragma once

#include <functional>
#include <list>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/Task.hpp"
#include "PolicyBase.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

class GeneticAlgorithm : public PolicyBase
{
  public:
    struct Parameters
    {
        mapping::MappingAssessorPtr solutionAssessor;
    };

    GeneticAlgorithm(const InfrastructureCPtr &infrastructure, Parameters &&parameters,
                     const logger::LoggerPtr &logger);

    NodeToTaskMapping buildNodeToTaskMapping(const TaskPtrVec &tasks) override;

  private:
    const Parameters parameters;

    const logger::LoggerPtr logger;
};

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
