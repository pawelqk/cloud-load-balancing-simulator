#pragma once

#include <string>

#include "Cloud/Infrastructure.hpp"
#include "Cloud/LoadBalancer/Policy/PolicyBase.hpp"
#include "Cloud/Task.hpp"
#include "Logger/Logger.hpp"
#include "Utility/RandomNumberGenerator.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace random
{

class RandomWithMigrationsAndPreemptions : public PolicyBase
{
  public:
    RandomWithMigrationsAndPreemptions(const InfrastructureCPtr &infrastructure,
                                       const utility::RandomNumberGeneratorPtr &randomNumberGenerator,
                                       const logger::LoggerPtr &logger);

    NodeToTaskMapping buildNodeToTaskMappingInternal(const TaskPtrVec &tasks) override;

    std::string toString() const override;

  private:
    const utility::RandomNumberGeneratorPtr randomNumberGenerator;
    const logger::LoggerPtr logger;
};

} // namespace random
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
