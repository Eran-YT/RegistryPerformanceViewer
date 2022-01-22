#include "CounterNames.hpp"

#include <windows.h>
#include <system_error>
#include <ranges>

namespace performance_data
{

CounterNames::CounterNames() :
    m_counter_names(get_counter_names())
{
}

std::wstring CounterNames::counter_name(const std::wstring& counter_id) const
{
    return m_counter_names.at(counter_id);
}

std::wstring CounterNames::counter_id(const std::wstring& counter_name) const
{
    for (const auto& [key, value] : m_counter_names) {
        if (value == counter_name) {
            return key;
        }
    }

    throw std::runtime_error("Failed to find counter ID");
}


std::set<std::wstring> CounterNames::counter_ids() const
{
    // This is because ranges::to is not implemented yet in the STL
    std::set<std::wstring> keys;
    for (const auto& key : m_counter_names | std::views::keys) {
        keys.insert(key);
    }

    return keys;
}

std::set<std::wstring> CounterNames::counter_names() const
{
    // This is because ranges::to is not implemented yet in the STL
    std::set<std::wstring> values;
    for (const auto& value : m_counter_names | std::views::values) {
        values.insert(value);
    }

    return values;
}

std::map<std::wstring, std::wstring> CounterNames::get_counter_names()
{
    std::vector<wchar_t> counter_data = get_counter_data(L"Counter");

    std::map<std::wstring, std::wstring> counter_names;
    size_t current_index = 0;

    while (current_index < counter_data.size()) {
        std::wstring counter_id(counter_data.data() + current_index);
        if (counter_id.empty()) {
            break;
        }
        current_index += counter_id.size() + 1;

        std::wstring counter_name(counter_data.data() + current_index);
        current_index += counter_name.size() + 1;

        counter_names[counter_id] = counter_name;
    }

    return counter_names;
}

std::vector<wchar_t> CounterNames::get_counter_data(const std::wstring& source)
{
    DWORD dwBufferSize = 0;

    LONG status = RegQueryValueEx(HKEY_PERFORMANCE_NLSTEXT, source.data(), NULL, NULL, NULL, &dwBufferSize);
    if (ERROR_SUCCESS != status) {
        throw std::system_error(std::error_code(GetLastError(), std::system_category()));
    }

    std::vector<wchar_t> data(dwBufferSize / 2);
    // Allocate the text buffer and query the text.
    status = RegQueryValueEx(HKEY_PERFORMANCE_NLSTEXT, source.data(), NULL, NULL, reinterpret_cast<LPBYTE>(data.data()), &dwBufferSize);
    if (ERROR_SUCCESS != status) {
        throw std::system_error(std::error_code(GetLastError(), std::system_category()));
    }

    return data;
}

}
