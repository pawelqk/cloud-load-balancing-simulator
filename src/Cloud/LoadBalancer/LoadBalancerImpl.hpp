#include "LoadBalancer.hpp"

namespace cloud
{
namespace loadbalancer
{

class LoadBalancerImpl : public LoadBalancer
{
  public:
    LoadBalancerImpl(const std::vector<Node> &nodes);

    void schedule(const std::vector<Task> &tasks) override;
    void tick() override;
    bool isIdle() const override;

  private:
    bool areNodesIdle() const;
    void reschedule();

    std::vector<Node> nodes;
    std::vector<Task> waitingTasks;
};

} // namespace loadbalancer
} // namespace cloud
