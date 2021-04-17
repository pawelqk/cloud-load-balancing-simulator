#include "Files.hpp"

#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>

namespace logger
{

namespace fs = std::filesystem;

Files::Files(const std::string &dirName, const std::string &filePrefix)
{
    fs::create_directories(dirName);
    file.open(dirName + "/" + prepareFileName(filePrefix), std::ios::app);
}

void Files::log(const char *message)
{
    file << message << std::endl;
}

std::string Files::prepareFileName(const std::string &filePrefix)
{
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::stringstream ss;
    ss << filePrefix << "_" << std::put_time(std::localtime(&now), "%Y-%m-%d_%X");
    return ss.str();
}

} // namespace logger
