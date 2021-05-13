#pragma once

#include <cstdint>
#include <list>

#include "ArtificialBeeColonyBase.hpp"
#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"
#include "Cloud/Task.hpp"
#include "Configuration/Instance.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace artificialbeecolony
{

class OfflineArtificialBeeColony : public PolicyBase
{
  public:
    OfflineArtificialBeeColony(const InfrastructureCPtr &infrastructure, const Parameters &parameters,
                               mapping::MappingAssessorPtr &&mappingAssessor, const configuration::Instance &instance,
                               const logger::LoggerPtr &logger);

    NodeToTaskMapping buildNodeToTaskMapping(const TaskPtrVec &tasks) override;

    std::string toString() const override;

  protected:
    const Parameters parameters;
    const mapping::MappingAssessorPtr mappingAssessor;

  private:
    const configuration::Instance instance;
};

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
