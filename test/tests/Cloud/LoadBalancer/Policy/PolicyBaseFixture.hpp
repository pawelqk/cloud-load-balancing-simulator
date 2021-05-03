
#include <gtest/gtest.h>

#include "mocks/InfrastructureMock.hpp"
#include "mocks/NodeMock.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace tests
{

struct PolicyBaseFixture : testing::Test
{
    void expectGettingNodes(const std::vector<NodePtr> &nodeMocks)
    {
        EXPECT_CALL(Const(*infrastructureMock), getNodes()).WillRepeatedly(testing::ReturnRef(nodeMocks));
    }

    const mocks::InfrastructureMockPtr infrastructureMock{std::make_shared<mocks::InfrastructureMock>()};
};

} // namespace tests
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
