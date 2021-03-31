#include "RandomNumberGenerator.hpp"
#include <random>

namespace utility
{

RandomNumberGenerator::RandomNumberGenerator() : std::mt19937(542512 /*std::random_device{}()*/)
{
}

RandomNumberGenerator &RandomNumberGenerator::getInstance()
{
    static RandomNumberGenerator randomNumberGenerator;
    return randomNumberGenerator;
}

} // namespace utility
