#pragma once
#include <string>
#include <vector>
#include <Windows.h>

#include "PerfStructs.hpp"

namespace performance_data
{
class CounterData final
{
public:
	explicit CounterData(const std::wstring& counter_id);

    ~CounterData() = default;

    [[nodiscard]] PerfInstances instances() const;

    CounterData(const CounterData&) = default;
    CounterData(CounterData&&) = default;
    CounterData& operator=(const CounterData&) = default;
    CounterData& operator=(CounterData&&) = default;

private:

    [[nodiscard]] static std::vector<Counter> parse_counter_block(const PERF_COUNTER_BLOCK* counter_block,
                                                                  const std::vector<Counter>& counters_template);

    [[nodiscard]] std::vector<Counter> parse_counter_definitions(uint32_t& current_index,
                                                                 const PERF_OBJECT_TYPE* object_type) const;

    [[nodiscard]] PerfInstance parse_instance(uint32_t& current_index,
                                              const std::vector<Counter>& counters_template) const;

    [[nodiscard]] PerfInstance parse_single_instance(uint32_t& current_index,
                                              const std::vector<Counter>& counters_template) const;

    [[nodiscard]] static std::vector<std::byte> get_counter_data(const std::wstring& counter_id);

    std::vector<std::byte> m_counter_data;
};

}