#pragma once

#include "../Instance/Instance.hpp"

namespace experiment
{

class Experiment
{
  public:
    Experiment(const instance::Instance &instance);

    void run();

  private:
    instance::Instance instance;

    logger::Logger logger;
};

} // namespace experiment
