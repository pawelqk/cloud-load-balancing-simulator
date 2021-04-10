#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <list>
#include <string_view>

#include "LoggingEndpoint.hpp"

namespace logger
{

class Logger
{
  public:
    Logger(const std::string &name);

    template <class... Args>
    void log(const char *msg, Args &&...args)
    {
        char buffer[700];
        const auto nameBuffer = nameTag.c_str();
        const auto nameBufferSize = nameTag.size();

        std::strcpy(buffer, nameBuffer);

        std::snprintf(buffer + nameBufferSize, sizeof(buffer) - nameBufferSize, msg, args...);

        for (auto &&endpoint : loggingEndpoints)
            endpoint->log(buffer);
    }

    static void addLoggingEndpoint(LoggingEndpointPtr &&loggingEndpoint);

  private:
    static std::list<LoggingEndpointPtr> loggingEndpoints;
    const std::string nameTag;
};

} // namespace logger
