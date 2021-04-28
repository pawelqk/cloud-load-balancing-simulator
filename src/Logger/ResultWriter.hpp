#pragma once

#include <fstream>
#include <memory>
#include <string>

#include "Experiment/Experiment.hpp"

namespace logger
{

class ResultWriter
{
  public:
    ResultWriter(const std::string &directoryName);

    void writeResults(const std::string &description, const std::vector<experiment::Experiment::Result> &results);

  private:
    std::string prepareFileName();
    std::string createColumns();
    std::string createResultRecord(const experiment::Experiment::Result &result);

    const std::string directoryName;
};

using ResultWriterPtr = std::unique_ptr<ResultWriter>;

} // namespace logger
