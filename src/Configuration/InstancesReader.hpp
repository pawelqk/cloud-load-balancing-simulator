#include <memory>
#include <vector>

#include "Instance.hpp"

namespace configuration
{

class InstancesReader
{
  public:
    virtual ~InstancesReader() = default;

    virtual std::vector<Instance> read() = 0;
};

using InstancesReaderPtr = std::unique_ptr<InstancesReader>;

} // namespace configuration
