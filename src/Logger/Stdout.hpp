#pragma once

#include "LoggingEndpoint.hpp"

namespace logger
{

class Stdout : public LoggingEndpoint
{
  public:
    void log(const char *message) override;
};

} // namespace logger
