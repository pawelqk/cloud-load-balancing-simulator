#include "Files.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace logger
{

namespace fs = std::filesystem;

Files::Files(const std::string &dirName, const std::string &filePrefix)
{
    const auto targetDirName = dirName + "/" + prepareStringTime();
    fs::create_directories(targetDirName);
    file.open(targetDirName + "/" + filePrefix, std::ios::app);
}

void Files::log(const char *message)
{
    file << message << std::endl;
}

std::string Files::prepareStringTime()
{
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::stringstream ss;
    ss << std::put_time(std::localtime(&now), "%Y-%m-%d_%X");
    return ss.str();
}

} // namespace logger
