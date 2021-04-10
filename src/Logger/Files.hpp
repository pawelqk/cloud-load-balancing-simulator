#pragma once

#include "LoggingEndpoint.hpp"
#include <fstream>

namespace logger
{

class Files : public LoggingEndpoint
{
  public:
    Files();

    void log(const char *message) override;

  private:
    std::string prepareFileName();

    std::ofstream file;
};

} // namespace logger
