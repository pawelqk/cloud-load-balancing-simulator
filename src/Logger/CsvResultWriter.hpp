#pragma once

#include <string>

#include "Experiment/Experiment.hpp"
#include "ResultWriter.hpp"

namespace logger
{

class CsvResultWriter : public ResultWriter
{
  public:
    CsvResultWriter(const std::string &directoryName);

    void writeResults(const std::string &description, const std::vector<experiment::Experiment::Result> &results);

  private:
    std::string getCurrentDate();
    std::string createColumns();
    std::string createResultRecord(const experiment::Experiment::Result &result);

    const std::string directoryPath;
};

} // namespace logger
