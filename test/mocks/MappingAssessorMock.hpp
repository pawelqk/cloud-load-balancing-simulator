#pragma once

#include <memory>

#include <gmock/gmock.h>

#include "Cloud/LoadBalancer/Mapping/MappingAssessor.hpp"
#include "Cloud/LoadBalancer/Policy/Policy.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace mapping
{
namespace mocks
{

class MappingAssessorMock : public testing::StrictMock<MappingAssessor>
{
  public:
    MOCK_METHOD(double, assess, (const policy::NodeToTaskMapping &), (override));
};

} // namespace mocks
} // namespace mapping
} // namespace loadbalancer
} // namespace cloud
