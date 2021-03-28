#include "ExperimentRunner.hpp"

#include <vector>

#include "Experiment.hpp"

namespace experiment
{

ExperimentRunner::ExperimentRunner(const std::vector<instance::Instance> &instances) : instances(instances)
{
}

void ExperimentRunner::run()
{
    Experiment e{instances[0]};
    e.run();
}

} // namespace experiment
