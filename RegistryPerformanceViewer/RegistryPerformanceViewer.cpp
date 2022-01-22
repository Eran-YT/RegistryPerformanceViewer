#include "CLI/App.hpp"
#include "CLI/Formatter.hpp"
#include "CLI/Config.hpp"
#include "RegistryPerformanceViewerLogic.hpp"

#include <codecvt>

int main(int argc, char** argv)
{
    CLI::App app{ "Extract information from HKEY_PERFORMANCE_DATA" };


    CLI::App* list_counters_subcommand = app.add_subcommand("list", "List all counters");
    auto list_counters = []() { RegistryPerformanceViewerLogic::list_all_counters(); };
    list_counters_subcommand->callback(list_counters);


    CLI::App* dump_counter_subcommand = app.add_subcommand("dump", "Dump information about a specific counter");
    std::string counter_name;
    dump_counter_subcommand->add_option("-c,--counter_name", counter_name, "Counter name");
    std::filesystem::path output_directory;
    dump_counter_subcommand->add_option("-o,--output_directory", output_directory, "Output directory");
    auto dump_counter = [&counter_name, &output_directory]() {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring counter_wide = converter.from_bytes(counter_name);

        RegistryPerformanceViewerLogic::dump_specific_counter(counter_wide, output_directory);
    };
    dump_counter_subcommand->callback(dump_counter);


    CLI::App* dump_all_counters_subcommand = app.add_subcommand("dump_all", "Dump information about all counters");
    dump_all_counters_subcommand->add_option("-o,--output_directory", output_directory, "Output directory");
    auto dump_all_counters = [&output_directory]() { RegistryPerformanceViewerLogic::dump_all_counters(output_directory); };
    dump_all_counters_subcommand->callback(dump_all_counters);

    app.require_subcommand(/* min */ 0, /* max */ 1);

    CLI11_PARSE(app, argc, argv);

    return 0;
}
