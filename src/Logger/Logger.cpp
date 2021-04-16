#include "Logger.hpp"

namespace logger
{

Logger::Logger(const std::string &name, const bool withDebug) : nameTag("[" + name + "] "), withDebug(withDebug)
{
}

void Logger::addLoggingEndpoint(LoggingEndpointPtr &&loggingEndpoint)
{
    loggingEndpoints.emplace_back(std::move(loggingEndpoint));
}

} // namespace logger
