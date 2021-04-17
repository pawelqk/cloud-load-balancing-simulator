#pragma once

#include <list>
#include <memory>
#include <string>
#include <vector>

namespace cloud
{

class Task
{
  public:
    virtual ~Task();

    virtual void work() = 0;
    virtual bool isDone() const = 0;
    virtual void performPreemption() = 0;
    virtual void performMigration() = 0;

    virtual std::uint32_t getId() const = 0;
    virtual std::uint32_t getMips() const = 0;
    virtual std::uint32_t getInitialLength() const = 0;
    virtual std::uint32_t getArrivalTime() const = 0;
    virtual std::uint32_t estimateTimeLeft() const = 0;
    virtual std::uint32_t estimateTimeLeftAfterMigration() const = 0;
    virtual std::uint32_t estimateTimeLeftAfterPreemption() const = 0;

    bool operator==(const Task &other) const;

    virtual std::string toString() const = 0;
};

using TaskPtr = std::shared_ptr<Task>;
using TaskPtrList = std::list<TaskPtr>;
using TaskPtrVec = std::vector<TaskPtr>;

} // namespace cloud
