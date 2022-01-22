#pragma once
#include <string>
#include <vector>
#include <map>

namespace performance_data
{

using CounterName = std::wstring;

struct Counter final
{
    CounterName name;
    uint32_t counter_type;
    uint32_t counter_size;
    uint32_t counter_offset;
    uint64_t counter_data;
};

struct PerfInstance final
{
    std::wstring name;
    std::vector<Counter> counters;
};

using PerfInstances = std::vector<PerfInstance>;

void print(const std::map<CounterName, PerfInstances>& instance_map);

}