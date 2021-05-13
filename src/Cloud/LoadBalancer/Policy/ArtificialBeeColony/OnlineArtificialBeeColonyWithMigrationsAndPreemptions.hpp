#pragma once

#include <cstdint>
#include <list>

#include "ArtificialBeeColonyBase.hpp"
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
namespace artificialbeecolony
{

class OnlineArtificialBeeColonyWithMigrationsAndPreemptions : public PolicyBase
{
  public:
    OnlineArtificialBeeColonyWithMigrationsAndPreemptions(const InfrastructureCPtr &infrastructure,
                                                          const Parameters &parameters,
                                                          mapping::MappingAssessorPtr &&mappingAssessor,
                                                          const logger::LoggerPtr &logger);

    NodeToTaskMapping buildNodeToTaskMapping(const TaskPtrVec &tasks) override;

    std::string toString() const override;

  protected:
    const Parameters parameters;
    const mapping::MappingAssessorPtr mappingAssessor;
};

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
