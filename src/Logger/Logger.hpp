#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <utility>
#include <vector>

#include "LoggingEndpoint.hpp"

namespace logger
{

class Logger
{
  public:
    Logger(const std::string &name, bool withDebug);

    template <class... Args>
    void debug(const char *msg, Args... args)
    {
        if (!withDebug)
            return;

        log("DEBUG: ", msg, args...);
    }

    template <class... Args>
    void info(const char *msg, Args... args)
    {
        log("INFO: ", msg, args...);
    }

    void addLoggingEndpoint(LoggingEndpointPtr &&loggingEndpoint);

  private:
    template <class... Args>
    void log(const std::string &prefix, const char *msg, Args... args)
    {
        char buffer[700];
        const auto nameBuffer = nameTag.c_str();
        const auto nameBufferSize = nameTag.size();
        const auto prefixBuffer = prefix.c_str();
        const auto prefixBufferSize = prefix.size();

        std::strcpy(buffer, nameBuffer);
        std::strcpy(buffer + nameBufferSize, prefixBuffer);

        std::snprintf(buffer + nameBufferSize + prefixBufferSize, sizeof(buffer) - nameBufferSize - prefixBufferSize,
                      msg, args...);

        for (auto &&endpoint : loggingEndpoints)
            endpoint->log(buffer);
    }

    std::vector<LoggingEndpointPtr> loggingEndpoints;
    const std::string nameTag;
    const bool withDebug;
};

using LoggerPtr = std::shared_ptr<Logger>;

} // namespace logger
