#pragma once

#include <memory>

namespace logger
{

class LoggingEndpoint
{
  public:
    virtual ~LoggingEndpoint() = default;

    virtual void log(const char *message) = 0;
};

using LoggingEndpointPtr = std::unique_ptr<LoggingEndpoint>;

} // namespace logger
