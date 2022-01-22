#pragma once

#include <filesystem>

class RegistryPerformanceViewerLogic final
{
public:
    static void list_all_counters();
    static void dump_all_counters(const std::filesystem::path& output_directory);
    static void dump_specific_counter(const std::wstring& counter_name, const std::filesystem::path& output_directory);

    RegistryPerformanceViewerLogic() = delete;
    ~RegistryPerformanceViewerLogic() = delete;
    RegistryPerformanceViewerLogic(const RegistryPerformanceViewerLogic&) = default;
    RegistryPerformanceViewerLogic(RegistryPerformanceViewerLogic&&) = default;
    RegistryPerformanceViewerLogic& operator=(const RegistryPerformanceViewerLogic&) = default;
    RegistryPerformanceViewerLogic& operator=(RegistryPerformanceViewerLogic&&) = default;
};