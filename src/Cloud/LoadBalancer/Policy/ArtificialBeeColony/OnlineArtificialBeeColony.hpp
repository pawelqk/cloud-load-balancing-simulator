#pragma once

#include <cstdint>

#include "ArtificialBeeColonyBase.hpp"
#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace artificialbeecolony
{

class OnlineArtificialBeeColony : public ArtificialBeeColonyBase
{
  public:
    OnlineArtificialBeeColony(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                              mapping::MappingAssessorPtr &&mappingAssessor, const logger::LoggerPtr &logger);

    std::string toString() const override;

  private:
    NodeToTaskMapping buildNodeToTaskMappingInternal(const TaskPtrVec &tasks) override;
    NodeToTaskMapping adjustSolutionWithExistingTasks(const NodeToTaskMapping &solution);
};

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
