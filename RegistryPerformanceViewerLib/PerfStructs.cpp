#include "PerfStructs.hpp"

#include <iostream>
#include <format>

namespace performance_data
{

void print(const std::map<CounterName, PerfInstances>& instance_map)
{
    for (const auto& [counter_name, instances] : instance_map) {
        std::wcout << std::format(L"Counter name: {}", counter_name) << std::endl;

        for (const PerfInstance& perf_instance : instances) {
            std::wcout << std::format(L"\tInstance name: {}", perf_instance.name) << std::endl;

            for (const Counter& counter : perf_instance.counters) {
                std::wcout << std::format(L"\t\t{} - {}", counter.name, counter.counter_data) << std::endl;
            }
        }
    }

}

}
