#pragma once
#include <string>
#include <vector>

namespace performance_data
{

struct Counter
{
    std::wstring name;
    uint32_t counter_type;
    uint32_t counter_size;
    uint32_t counter_offset;
    uint64_t counter_data;
};

struct PerfInstance
{
    std::wstring name;
    std::vector<Counter> counters;
};

using PerfInstances = std::vector<PerfInstance>;

void print(const PerfInstances& instances);

}