#include <vector>

#include "../Instance/Instance.hpp"

namespace experiment
{

class ExperimentRunner
{
  public:
    ExperimentRunner(const std::vector<instance::Instance> &instances);

    void run();

  private:
    const std::vector<instance::Instance> instances;
};

} // namespace experiment
