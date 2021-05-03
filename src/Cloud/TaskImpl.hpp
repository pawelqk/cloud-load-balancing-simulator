#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "Task.hpp"

namespace cloud
{

class TaskImpl : public Task
{
  public:
    explicit TaskImpl(const std::uint32_t id, const std::uint32_t mips, const std::uint32_t initialLength,
                      const std::uint32_t arrivalTime, const double penaltyFactor);
    TaskImpl() = delete;
    TaskImpl(const TaskImpl &) = delete;
    TaskImpl(TaskImpl &&) = delete;
    TaskImpl &operator=(const TaskImpl &) = delete;
    TaskImpl &operator=(TaskImpl &&) = delete;

    void work() override;
    bool isDone() const override;
    void performPreemption() override;
    void performMigration() override;

    std::uint32_t getId() const override;
    std::uint32_t getMips() const override;
    std::uint32_t getInitialLength() const override;
    std::uint32_t getArrivalTime() const override;
    std::uint32_t getElapsedTime() const override;
    std::uint32_t estimateTimeLeft() const override;
    std::uint32_t estimateTimeLeftAfterMigration() const override;
    std::uint32_t estimateTimeLeftAfterPreemption() const override;

    std::string toString() const override;

  private:
    std::uint32_t calculateMigrationPenalty() const;
    std::uint32_t estimateTime(const std::uint32_t lengthToEstimate) const;

    const std::uint32_t id;
    const std::uint32_t mips;
    const std::uint32_t initialLength;
    const std::uint32_t arrivalTime;
    const double penaltyFactor;

    std::uint32_t currentLength;
    std::uint32_t elapsedTime;
};

} // namespace cloud
