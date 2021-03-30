#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string_view>

namespace logger
{

class Logger
{
  public:
    Logger(const std::string &name);

    template <class... Args>
    void log(const char *msg, Args... args)
    {
        char buffer[700];
        const auto nameBuffer = nameTag.c_str();
        const auto nameBufferSize = nameTag.size();

        std::strcpy(buffer, nameBuffer);

        std::snprintf(buffer + nameBufferSize, sizeof(buffer) - nameBufferSize, msg, args...);

        std::cout << buffer << std::endl;
    }

  private:
    const std::string nameTag;
};

} // namespace logger
