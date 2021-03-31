#pragma once

#include <cstdint>
#include <random>

namespace utility
{

class RandomNumberGenerator : public std::mt19937
{
  public:
    static RandomNumberGenerator &getInstance();

    RandomNumberGenerator(const RandomNumberGenerator &) = delete;
    RandomNumberGenerator(RandomNumberGenerator &&) = delete;

    RandomNumberGenerator &operator=(const RandomNumberGenerator &) = delete;
    RandomNumberGenerator &operator=(RandomNumberGenerator &&) = delete;

  private:
    RandomNumberGenerator();
};

} // namespace utility
