#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Cloud.hpp"
#include "LoadBalancer/Strategy/Strategy.hpp"

namespace cloud
{

class CloudBuilder
{
  public:
    std::unique_ptr<Cloud> build(const std::vector<std::uint32_t> &nodesMips);
};

} // namespace cloud
