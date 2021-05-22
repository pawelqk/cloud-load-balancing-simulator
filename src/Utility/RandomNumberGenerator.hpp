#pragma once

#include <cstdint>
#include <memory>
#include <random>

namespace utility
{

class RandomNumberGenerator : public std::mt19937_64
{
  public:
    RandomNumberGenerator() = delete;
    RandomNumberGenerator(const RandomNumberGenerator &) = delete;
    RandomNumberGenerator(RandomNumberGenerator &&) = delete;
    RandomNumberGenerator(const std::uint_fast64_t seed);

    RandomNumberGenerator &operator=(const RandomNumberGenerator &) = delete;
    RandomNumberGenerator &operator=(RandomNumberGenerator &&) = delete;
};

using RandomNumberGeneratorPtr = std::shared_ptr<RandomNumberGenerator>;

} // namespace utility
