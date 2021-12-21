#include "PerfStructs.hpp"

#include <iostream>
#include <format>

namespace performance_data
{

void print(const PerfInstances& instances)
{
    for (const PerfInstance& perf_instance : instances) {
        std::wcout << std::format(L"Instance name: {}", perf_instance.name) << std::endl;

        for (const Counter& counter : perf_instance.counters) {
            std::wcout << std::format(L"\t{} - {}", counter.name, counter.counter_data) << std::endl;
        }
    }
}

}
