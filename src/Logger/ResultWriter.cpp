#include "ResultWriter.hpp"

#include <filesystem>
#include <sstream>
#include <string>

namespace logger
{

namespace fs = std::filesystem;

ResultWriter::ResultWriter(const std::string &directoryName) : directoryPath(directoryName + "/" + getCurrentDate())
{
    fs::create_directories(directoryPath);
}

void ResultWriter::writeResults(const std::string &description,
                                const std::vector<experiment::Experiment::Result> &results)
{
    if (results.empty())
        return;

    std::ofstream file{directoryPath + "/" + description};
    file << createColumns() << std::endl;

    for (auto &&result : results)
        file << createResultRecord(result) << std::endl;
}

std::string ResultWriter::getCurrentDate()
{
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::stringstream ss;
    ss << std::put_time(std::localtime(&now), "%Y-%m-%d_%X");
    return ss.str();
}

std::string ResultWriter::createColumns()
{
    return "instance_id|seed|penalty_factor|makespan|flowtime";
}

std::string ResultWriter::createResultRecord(const experiment::Experiment::Result &result)
{
    return std::to_string(result.instanceId) + "|" + std::to_string(result.seed) + "|" +
           std::to_string(result.penaltyFactor) + "|" + std::to_string(result.makespan) + "|" +
           std::to_string(result.flowtime);
}

} // namespace logger
