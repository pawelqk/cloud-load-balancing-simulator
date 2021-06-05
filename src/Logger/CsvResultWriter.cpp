#include "CsvResultWriter.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace logger
{

namespace fs = std::filesystem;

CsvResultWriter::CsvResultWriter(const std::string &directoryName)
    : directoryPath(directoryName + "/" + getCurrentDate())
{
    fs::create_directories(directoryPath);
}

void CsvResultWriter::writeResults(const std::string &description,
                                   const std::vector<experiment::Experiment::Result> &results)
{
    if (results.empty())
        return;

    std::ofstream f{directoryPath + "/" + description};
    f << createColumns() << std::endl;

    for (auto &&result : results)
        f << createResultRecord(result) << std::endl;
}

std::string CsvResultWriter::getCurrentDate()
{
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::stringstream ss;
    ss << std::put_time(std::localtime(&now), "%Y-%m-%d_%X");
    return ss.str();
}

std::string CsvResultWriter::createColumns()
{
    return "instance_id|seed|penalty_factor|makespan|flowtime";
}

std::string CsvResultWriter::createResultRecord(const experiment::Experiment::Result &result)
{
    return std::to_string(result.instanceId) + "|" + std::to_string(result.seed) + "|" +
           std::to_string(result.penaltyFactor) + "|" + std::to_string(result.makespan) + "|" +
           std::to_string(result.flowtime);
}

} // namespace logger
