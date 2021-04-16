#pragma once

#include <vector>

#include "../Instance/Instance.hpp"

namespace experiment
{

class ExperimentRunner
{
  public:
    ExperimentRunner(const std::vector<instance::Instance> &instances);

    void run(bool withDebug, bool withStdout);

  private:
    const std::vector<instance::Instance> instances;
};

} // namespace experiment
