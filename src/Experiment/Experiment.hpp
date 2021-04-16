#pragma once

#include "../Instance/Instance.hpp"

namespace experiment
{

class Experiment
{
  public:
    Experiment(const instance::Instance &instance, const logger::LoggerPtr &logger);

    void run();

  private:
    instance::Instance instance;

    const logger::LoggerPtr logger;
};

} // namespace experiment
