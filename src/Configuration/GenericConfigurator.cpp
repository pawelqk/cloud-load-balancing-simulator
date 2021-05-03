#include "GenericConfigurator.hpp"

#include "Cloud/LoadBalancer/Policy/Random/RandomBuilder.hpp"
#include "Cloud/LoadBalancer/Policy/RoundRobin/RoundRobinBuilder.hpp"
#include "Cloud/LoadBalancer/Policy/ShortestRemainingTimeFirst/ShortestRemainingTimeFirstBuilder.hpp"

namespace configuration
{

GenericConfigurator::GenericConfigurator(const Policy policy) : policy(policy)
{
}

cloud::loadbalancer::policy::PolicyBuilderPtr GenericConfigurator::configure(const nlohmann::json &configuration)
{
    std::string type;
    try
    {
        type = configuration.at("type");
    }
    catch (nlohmann::json::out_of_range &e)
    {
        throw InvalidConfigurationException{e.what()};
    }

    if (type == "Offline")
        return build(PolicyConfiguration::Offline);
    else if (type == "Online")
        return build(PolicyConfiguration::Online);
    else if (type == "Online with migrations")
        return build(PolicyConfiguration::OnlineWithMigrations);
    else if (type == "Online with migrations and preemptions")
        return build(PolicyConfiguration::OnlineWithMigrationsAndPreemptions);
    else
        return nullptr;
}

cloud::loadbalancer::policy::PolicyBuilderPtr GenericConfigurator::build(const PolicyConfiguration policyConfiguration)
{
    switch (policy)
    {
    case Policy::Random:
        return std::make_shared<cloud::loadbalancer::policy::random::RandomBuilder>(policyConfiguration);
    case Policy::RoundRobin:
        return std::make_shared<cloud::loadbalancer::policy::roundrobin::RoundRobinBuilder>(policyConfiguration);
    case Policy::ShortestRemainingTimeFirst:
        return std::make_shared<
            cloud::loadbalancer::policy::shortestremainingtimefirst::ShortestRemainingTimeFirstBuilder>(
            policyConfiguration);
    default:
        return nullptr;
    }
}

} // namespace configuration
