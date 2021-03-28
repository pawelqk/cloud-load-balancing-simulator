#include "LoadBalancer.hpp"

namespace cloud
{
namespace loadbalancer
{

class LoadBalancerImpl : public LoadBalancer
{
  public:
    LoadBalancerImpl(const std::vector<Node> &nodes);

    void schedule(const TaskSet &tasks) override;
    void tick() override;
    bool isIdle() const override;

  private:
    bool areNodesIdle() const;
    void scheduleWaitingTasks();

    std::vector<Node> nodes;
    TaskSet waitingTasks;
};

} // namespace loadbalancer
} // namespace cloud
