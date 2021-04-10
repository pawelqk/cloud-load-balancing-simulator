#include "Logger.hpp"

namespace logger
{

Logger::Logger(const std::string &name) : nameTag("[" + name + "]: ")
{
}

void Logger::addLoggingEndpoint(LoggingEndpointPtr &&loggingEndpoint)
{
    loggingEndpoints.emplace_back(std::move(loggingEndpoint));
}

std::list<LoggingEndpointPtr> Logger::loggingEndpoints;

} // namespace logger
