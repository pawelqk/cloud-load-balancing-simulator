#pragma once

#include "LoggingEndpoint.hpp"
#include <fstream>

namespace logger
{

class Files : public LoggingEndpoint
{
  public:
    Files(const std::string &dirName, const std::string &filePrefix);

    void log(const char *message) override;

  private:
    std::string prepareFileName(const std::string &filePrefix);

    std::ofstream file;
};

} // namespace logger
