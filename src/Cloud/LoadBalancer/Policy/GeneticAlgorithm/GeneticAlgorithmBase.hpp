#pragma once

#include <cstdint>

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace geneticalgorithm
{

struct Parameters
{
    double mutationProbability;
    std::uint32_t populationSize;
    std::uint32_t maxIterations;
    double eliteIndividualsRatio;
};

} // namespace geneticalgorithm
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
