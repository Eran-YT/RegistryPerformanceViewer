#include "RegistryPerformanceViewerLogic.hpp"
#include "..\RegistryPerformanceViewerLib\CounterNames.hpp"

#include <iostream>
#include <fstream>
#include <codecvt>

#include "../RegistryPerformanceViewerLib/CounterData.hpp"

#include "csv.hpp"
#include "Utils.hpp"

using namespace performance_data;

void RegistryPerformanceViewerLogic::list_all_counters()
{
    CounterNames counters;

    for (const std::wstring& counter_name : counters.counter_names()) {
        std::wcout << counter_name << std::endl;
    }
}

void RegistryPerformanceViewerLogic::dump_all_counters(const std::filesystem::path& output_directory)
{
    CounterNames counters;

    for (const std::wstring& counter_name : counters.counter_names()) {
        dump_specific_counter(counter_name, output_directory / Utils::fix_path(counter_name));
    }
}

void RegistryPerformanceViewerLogic::dump_specific_counter(const std::wstring& counter_name, const std::filesystem::path& output_directory)
{
    std::filesystem::create_directories(output_directory);

    CounterNames counters;

    CounterData counter(counters.counter_id(counter_name));

    std::map<CounterName, PerfInstances> instance_map = counter.instances();
    if (instance_map.empty()) {
        return;
    }

    for (const auto& [counter_name, instances] : instance_map) {
        
        std::ofstream output_file(output_directory / (counter_name + L".csv"));
        auto writer = csv::make_csv_writer(output_file);
        std::vector<std::string> csv_headers_row;
        csv_headers_row.push_back("Instance name");
        if (instances.empty()) {
            continue;
        }

        for (const Counter& counter : instances[0].counters) {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::string name_narrow = converter.to_bytes(counter.name);
            csv_headers_row.push_back(name_narrow);
        }
        writer << csv_headers_row;

        for (const PerfInstance& instance : instances) {

            std::vector<std::string> csv_row;

            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::string name_narrow = converter.to_bytes(instance.name);
            csv_row.push_back(name_narrow);

            for (const Counter& counter : instance.counters) {
                csv_row.push_back(std::to_string(counter.counter_data));
            }
            writer << csv_row;
        }
    }

}

