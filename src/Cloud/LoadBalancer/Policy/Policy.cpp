#include "Policy.hpp"

#include <sstream>

namespace cloud
{
namespace loadbalancer
{
namespace policy
{

std::string toString(const NodeToTaskMapping &mapping)
{
    std::stringstream ss;
    for (auto &&[nodeId, tasks] : mapping)
    {
        std::stringstream ssIn;
        ssIn << "Node=" << nodeId << ": [";
        for (auto &&task : tasks)
            ssIn << task->getId() << ", ";

        auto nodeLine = ssIn.str();

        const auto commaToDeletePos = nodeLine.rfind(", ");
        if (commaToDeletePos != std::string::npos)
            nodeLine.erase(commaToDeletePos);

        ss << nodeLine << "]\n";
    }

    auto output = ss.str();

    const auto newLineToDeletePos = output.rfind('\n');
    if (newLineToDeletePos != std::string::npos)
        output.erase(newLineToDeletePos);

    return output;
}

void PrintTo(const NodeToTaskMapping &mapping, std::ostream *os)
{
    *os << toString(mapping);
}

Policy::~Policy() = default;

} // namespace policy
} // namespace loadbalancer
} // namespace cloud
