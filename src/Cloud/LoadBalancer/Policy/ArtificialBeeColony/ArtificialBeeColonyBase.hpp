#pragma once

#include <cstdint>

#include "Cloud/LoadBalancer/Policy/Policy.hpp"

namespace cloud
{
namespace loadbalancer
{
namespace policy
{
namespace artificialbeecolony
{

struct Parameters
{
    std::uint32_t populationSize;
    std::uint32_t maxIterationsWithoutChange;
    std::uint32_t maxIterations;
};

struct BeeSolution
{
    NodeToTaskMapping solution;
    std::uint32_t counter;
};

struct BestSolution
{
    NodeToTaskMapping solution;
    double value;
};

} // namespace artificialbeecolony
} // namespace policy
} // namespace loadbalancer
} // namespace cloud
