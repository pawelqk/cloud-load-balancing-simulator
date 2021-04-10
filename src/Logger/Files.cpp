#include "Files.hpp"

#include <filesystem>
#include <fstream>

namespace logger
{

namespace fs = std::filesystem;

Files::Files()
{
    fs::create_directory("log");
    std::ofstream file("log/" + prepareFileName(), std::ios::app);
}

void Files::log(const char *message)
{
    file << message << '\n';
}

std::string Files::prepareFileName()
{
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::stringstream ss;
    ss << "log_" << std::put_time(std::localtime(&now), "%Y-%m-%d_%X");
    return ss.str();
}

} // namespace logger
