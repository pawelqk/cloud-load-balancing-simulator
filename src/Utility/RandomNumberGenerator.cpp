#include "RandomNumberGenerator.hpp"
#include <random>

namespace utility
{

RandomNumberGenerator::RandomNumberGenerator(const std::uint_fast64_t seed) : std::mt19937_64(seed)
{
}

} // namespace utility
