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

class OnlineArtificialBeeColonyWithMigrationsAndPreemptions : public ArtificialBeeColonyBase
{
  public:
    OnlineArtificialBeeColonyWithMigrationsAndPreemptions(
        const InfrastructureCPtr &infrastructure, const Parameters &parameters,
        mapping::MappingAssessorPtr &&mappingAssessor, const utility::RandomNumberGeneratorPtr &randomNumberGenerator,
        const logger::LoggerPtr &logger);

    std::string toString() const override;

  private:
    NodeToTaskMapping buildNodeToTaskMappingInternal(const TaskPtrVec &tasks) override;
};

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
