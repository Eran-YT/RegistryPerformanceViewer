#include "CounterData.hpp"

#include <system_error>
#include <iostream>
#include <format>
#include "CounterNames.hpp"

namespace performance_data
{

CounterNames counter_names;

CounterData::CounterData(const std::wstring& counter_id) :
    m_counter_data(get_counter_data(counter_id))
{
}

std::map<CounterName, PerfInstances> CounterData::instances() const
{
    std::map<CounterName, PerfInstances> instance_map;

        const auto* perf_data_block = reinterpret_cast<const PERF_DATA_BLOCK*>(m_counter_data.data());

        uint32_t current_object_index = perf_data_block->HeaderLength;
        for (uint32_t current_object = 0; current_object < perf_data_block->NumObjectTypes; current_object++) {
            const auto* object_type = reinterpret_cast<const PERF_OBJECT_TYPE*>(m_counter_data.data() + current_object_index);

            uint32_t current_index = current_object_index + object_type->HeaderLength;
            std::vector<Counter> counters = parse_counter_definitions(current_index, object_type);

            if (object_type->NumInstances != PERF_NO_INSTANCES) {
                for (LONG current_instance = 0; current_instance < object_type->NumInstances; current_instance++) {
                    instances.push_back(parse_instance(current_index, counters));
                }
            } else {
                instances.push_back(parse_single_instance(current_index, counters));
            }

        CounterName counter_name = counter_names.counter_name(std::to_wstring(object_type->ObjectNameTitleIndex));
        instance_map[counter_name] = instances;

        current_object_index += object_type->TotalByteLength;
    }

    return instance_map;
}

    std::vector<Counter> CounterData::parse_counter_block(const PERF_COUNTER_BLOCK* counter_block,
        const std::vector<Counter>& counters_template)
    {
        std::vector<Counter> current_counters = counters_template;
        const char* counter_block_start = reinterpret_cast<const char*>(counter_block);

        for (Counter& current_counter : current_counters) {
            const void* counter_data_address = reinterpret_cast<const void*>(counter_block_start +
                current_counter.counter_offset);

            if (current_counter.counter_size == 4) {
                const auto* counter_data = reinterpret_cast<const uint32_t*>(counter_data_address);
                current_counter.counter_data = *counter_data;
            }

            if (current_counter.counter_size == 8) {
                const auto* counter_data = reinterpret_cast<const uint64_t*>(counter_data_address);
                current_counter.counter_data = *counter_data;
            }
        }

        return current_counters;
    }

    std::vector<Counter> CounterData::parse_counter_definitions(uint32_t& current_index,
        const PERF_OBJECT_TYPE* object_type) const
    {
        static CounterNames counter_names;

        std::vector<Counter> counters;
        for (uint32_t current_counter = 0; current_counter < object_type->NumCounters; current_counter++) {
            const auto* counter_definition = reinterpret_cast<const PERF_COUNTER_DEFINITION*>(
                m_counter_data.data() + current_index);

            Counter counter{
                .name = counter_names.counter_name(std::to_wstring(counter_definition->CounterNameTitleIndex)),
                .counter_type = counter_definition->CounterType,
                .counter_size = counter_definition->CounterSize,
                .counter_offset = counter_definition->CounterOffset,
            };

            counters.push_back(counter);
            current_index += counter_definition->ByteLength;
        }

        return counters;
    }

    PerfInstance CounterData::parse_instance(uint32_t& current_index,
        const std::vector<Counter>& counters_template) const
    {
        const auto* instance_definition = reinterpret_cast<const PERF_INSTANCE_DEFINITION*>(m_counter_data.data() + current_index);
        std::wstring instance_name(reinterpret_cast<const wchar_t*>(instance_definition) + (instance_definition->NameOffset / sizeof(wchar_t)));

        current_index += instance_definition->ByteLength;

        const auto* counter_block = reinterpret_cast<const PERF_COUNTER_BLOCK*>(m_counter_data.data() + current_index);

        PerfInstance instance{
            .name = instance_name,
            .counters = parse_counter_block(counter_block, counters_template)
        };

        current_index += counter_block->ByteLength;

        return instance;
    }

    PerfInstance CounterData::parse_single_instance(uint32_t& current_index,
        const std::vector<Counter>& counters_template) const
    {
        const auto* counter_block = reinterpret_cast<const PERF_COUNTER_BLOCK*>(m_counter_data.data() + current_index);

        PerfInstance instance{
            .name = L"noname",
            .counters = parse_counter_block(counter_block, counters_template)
        };

        current_index += counter_block->ByteLength;

        return instance;
    }

    std::vector<std::byte> CounterData::get_counter_data(const std::wstring& counter_id)
    {
        DWORD dwBufferSize = 0;        // Size of buffer, used to increment buffer size
        DWORD dwSize = 0;              // Size of buffer, used when calling RegQueryValueEx
        LONG status = ERROR_MORE_DATA;

        std::vector<std::byte> data;

        do {
            data.resize(dwBufferSize);
            status = RegQueryValueExW(HKEY_PERFORMANCE_DATA,
                counter_id.data(),
                NULL,
                NULL,
                reinterpret_cast<LPBYTE>(data.data()),
                &dwSize);

            // Contents of dwSize is unpredictable if RegQueryValueEx fails, which is why
            // you need to increment dwBufferSize and use it to set dwSize.
            dwBufferSize += 16384;

            dwSize = dwBufferSize;
        } while (status == ERROR_MORE_DATA);

        if (status != ERROR_SUCCESS) {
            throw std::system_error(std::error_code(GetLastError(), std::system_category()));
        }

        // This flushes the HKEY_PERFORMANCE_DATA buffers and frees the performance dlls
        RegCloseKey(HKEY_PERFORMANCE_DATA);

        return data;
    }

}

