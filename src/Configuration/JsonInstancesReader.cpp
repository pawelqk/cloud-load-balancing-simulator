#include "JsonInstancesReader.hpp"
#include "ConfigurationReader.hpp"

namespace configuration
{
JsonInstancesReader::JsonInstancesReader(const nlohmann::json &json) : json(json)
{
}

std::vector<Instance> JsonInstancesReader::read()
{
    std::vector<configuration::Instance> instances;
    instances.reserve(json.size());
    try
    {
        for (auto &&instanceData : json)
        {
            std::map<std::uint32_t, configuration::TaskDataVec> tasks;
            const auto &tasksData = instanceData.at("tasks");
            for (auto i = 0u; i < tasksData.size(); ++i)
            {
                tasks[tasksData[i].at("arrivalTime")].emplace_back(i, tasksData[i].at("requiredMips"),
                                                                   tasksData[i].at("length"));
            }

            const auto &instanceId = instanceData.at("id");
            const auto &nodesMips = instanceData.at("nodesMips");
            configuration::NodeDataVec nodesData(nodesMips.size());
            for (auto i = 0u; i < nodesMips.size(); ++i)
                nodesData[i] = {i, nodesMips[i]};

            instances.emplace_back(instanceId, tasks, nodesData);
        }
    }
    catch (nlohmann::json::out_of_range &e)
    {
        throw InvalidConfigurationException{e.what()};
    }

    return instances;
}

} // namespace configuration
