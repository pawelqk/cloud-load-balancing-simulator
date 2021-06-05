#pragma once

#include <memory>
#include <vector>

#include <nlohmann/json.hpp>

#include "InstancesReader.hpp"

namespace configuration
{

class JsonInstancesReader : public InstancesReader
{
  public:
    JsonInstancesReader(const nlohmann::json &json);

    std::vector<Instance> read() override;

  private:
    const nlohmann::json json;
};

} // namespace configuration
