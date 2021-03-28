#include <vector>

#include "LoadBalancer/LoadBalancer.hpp"
#include "Node.hpp"
#include "Task.hpp"

namespace cloud
{

class Cloud
{
  public:
    Cloud(loadbalancer::LoadBalancerPtr &&loadBalancer);

    void insertTasks(const std::vector<Task> &tasks);
    void tick();

    bool isIdle() const;

  private:
    const loadbalancer::LoadBalancerPtr loadBalancer;
};

} // namespace cloud
