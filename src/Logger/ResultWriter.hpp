#pragma once

#include <memory>
#include <string>

#include "Experiment/Experiment.hpp"

namespace logger
{

class ResultWriter
{
  public:
    virtual ~ResultWriter() = default;

    virtual void writeResults(const std::string &description,
                              const std::vector<experiment::Experiment::Result> &results) = 0;
};

using ResultWriterPtr = std::unique_ptr<ResultWriter>;

} // namespace logger
