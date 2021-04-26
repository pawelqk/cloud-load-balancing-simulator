#pragma once

#include <cstdint>
#include <random>

namespace utility
{

class RandomNumberGenerator : public std::mt19937_64
{
  public:
    static RandomNumberGenerator &getInstance();
    static RandomNumberGenerator &getInstance(const std::uint_fast64_t seed);

    RandomNumberGenerator() = delete;
    RandomNumberGenerator(const RandomNumberGenerator &) = delete;
    RandomNumberGenerator(RandomNumberGenerator &&) = delete;

    RandomNumberGenerator &operator=(const RandomNumberGenerator &) = delete;
    RandomNumberGenerator &operator=(RandomNumberGenerator &&) = delete;

  private:
    RandomNumberGenerator(const std::uint_fast64_t seed);
};

} // namespace utility
