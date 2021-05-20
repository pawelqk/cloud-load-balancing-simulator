#include <memory>

#include <gtest/gtest.h>

#include "Cloud/LoadBalancer/Policy/GeneticAlgorithm/Individual.hpp"
#include "Cloud/LoadBalancer/Policy/Policy.hpp"
#include "Utility/RandomNumberGenerator.hpp"
#include "mocks/MappingAssessorMock.hpp"
#include "mocks/NodeMock.hpp"
#include "mocks/TaskMock.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{
namespace tests
{

using testing::_;
using testing::Return;

struct CrossoverShould : testing::Test
{
    void SetUp() override
    {
        EXPECT_CALL(*mappingAssessorMock, assess(_)).WillRepeatedly(Return(0.0));
    }
    const std::shared_ptr<mapping::mocks::MappingAssessorMock> mappingAssessorMock{
        std::make_shared<mapping::mocks::MappingAssessorMock>()};
    const utility::RandomNumberGeneratorPtr randomNumberGenerator{std::make_shared<utility::RandomNumberGenerator>(0)};
};

TEST_F(CrossoverShould, HandleOneTaskChromosomes)
{
    const mocks::TaskMockPtr taskMock = std::make_shared<mocks::TaskMock>(0);
    const mocks::NodeMockPtr nodeMock = std::make_shared<mocks::NodeMock>(0);
    const NodeToTaskMapping mapping{{0, {taskMock}}};
    const Individual leftParent{mapping, nullptr, mappingAssessorMock, randomNumberGenerator};
    const Individual rightParent{mapping, nullptr, mappingAssessorMock, randomNumberGenerator};

    const std::vector<Individual::Gene> expectedOffspring{{0, 0}};
    ASSERT_EQ(Individual::crossover(leftParent, rightParent).getChromosome(), expectedOffspring);
}

} // namespace tests
} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
