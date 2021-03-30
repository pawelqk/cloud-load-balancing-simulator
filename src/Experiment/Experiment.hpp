#pragma once

#include "../Instance/Instance.hpp"

namespace experiment
{

class Experiment
{
  public:
    Experiment(const instance::Instance &instances);

    void run();

  private:
    const instance::Instance &instance;

    logger::Logger logger;
};

} // namespace experiment
