#include "RandomNumberGenerator.hpp"
#include <random>

namespace utility
{

RandomNumberGenerator::RandomNumberGenerator(const std::uint_fast64_t seed) : std::mt19937_64(seed)
{
}

RandomNumberGenerator &RandomNumberGenerator::getInstance()
{
    return getInstance(std::random_device{}());
}

RandomNumberGenerator &RandomNumberGenerator::getInstance(const std::uint_fast64_t seed)
{
    thread_local RandomNumberGenerator randomNumberGenerator{seed};
    return randomNumberGenerator;
}

} // namespace utility
