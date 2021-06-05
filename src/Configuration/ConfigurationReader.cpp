#include "ConfigurationReader.hpp"

namespace configuration
{

InvalidConfigurationException::InvalidConfigurationException(const std::string &msg) : std::runtime_error(msg)
{
}

std::string toString(const PolicyConfiguration policyConfiguration)
{
    switch (policyConfiguration)
    {
    case PolicyConfiguration::Offline:
        return "Offline";
    case PolicyConfiguration::Online:
        return "Online";
    case PolicyConfiguration::OnlineWithMigrationsAndPreemptions:
        return "OnlineWithMigrationsAndPreemptions";
    }

    return "";
}

std::string toString(const Assessment assessment)
{
    switch (assessment)
    {
    case Assessment::Makespan:
        return "Makespan";
    case Assessment::Flowtime:
        return "Flowtime";
    }

    return "";
}

ConfigurationReader::~ConfigurationReader() = default;

} // namespace configuration
